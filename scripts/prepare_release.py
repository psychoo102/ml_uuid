"""Prepare distinctly named release assets using only the Python standard library."""
import argparse
import hashlib
from pathlib import Path
import shutil
import zipfile

BINARIES = ("ml_uuid-win32.dll", "ml_uuid-win64.dll", "ml_uuid-linux-x64.so")


def require_file(path):
    if not path.is_file() or path.stat().st_size == 0:
        raise ValueError(f"Missing or empty release input: {path}")


def stage(build_dir, asset_name, output_dir=Path("dist")):
    if asset_name not in BINARIES:
        raise ValueError(f"Unsupported release asset: {asset_name}")
    extension = Path(asset_name).suffix
    matches = sorted(Path(build_dir).rglob("ml_uuid" + extension))
    if len(matches) != 1:
        raise ValueError(f"Expected exactly one ml_uuid{extension} in {build_dir}; found {len(matches)}")
    require_file(matches[0])
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    destination = output_dir / asset_name
    shutil.copyfile(matches[0], destination)
    return destination


def finalize(asset_dir, source_dir=Path(".")):
    asset_dir, source_dir = Path(asset_dir), Path(source_dir)
    # Check the complete matrix before any release API call.
    for name in BINARIES:
        require_file(asset_dir / name)

    notices = [source_dir / "LICENSE", source_dir / "THIRD_PARTY_NOTICES.md"]
    license_dir = source_dir / "LICENSES"
    if not license_dir.is_dir():
        raise ValueError("Missing LICENSES directory")
    notices.extend(sorted(path for path in license_dir.rglob("*") if path.is_file()))
    for name in ("Boost-1.0.txt", "Lua-5.1.txt", "MTA-Public-SDK-reference.txt"):
        require_file(license_dir / name)
    for path in notices:
        require_file(path)

    archive = asset_dir / "ml_uuid-notices.zip"
    with zipfile.ZipFile(archive, "w", compression=zipfile.ZIP_DEFLATED) as package:
        for path in notices:
            package.write(path, arcname=path.relative_to(source_dir).as_posix())

    lines = []
    for name in (*BINARIES, archive.name):
        digest = hashlib.sha256((asset_dir / name).read_bytes()).hexdigest()
        lines.append(f"{digest}  {name}\n")
    (asset_dir / "SHA256SUMS").write_text("".join(lines), encoding="utf-8", newline="\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    stage_parser = commands.add_parser("stage")
    stage_parser.add_argument("build_dir", type=Path)
    stage_parser.add_argument("asset_name", choices=BINARIES)
    final_parser = commands.add_parser("finalize")
    final_parser.add_argument("asset_dir", type=Path)
    args = parser.parse_args()
    if args.command == "stage":
        stage(args.build_dir, args.asset_name)
    else:
        finalize(args.asset_dir)


if __name__ == "__main__":
    main()
