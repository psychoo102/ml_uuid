"""Release packaging regressions using synthetic inputs, independent of MTA."""
import hashlib
from pathlib import Path
import tempfile
import unittest
import zipfile

from prepare_release import BINARIES, finalize, stage


class ReleasePackagingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.assets = self.root / "assets"
        self.assets.mkdir()
        for name in BINARIES:
            (self.assets / name).write_bytes(name.encode())
        self.source = self.root / "source"
        (self.source / "LICENSES").mkdir(parents=True)
        for name in ("LICENSE", "THIRD_PARTY_NOTICES.md",
                     "LICENSES/Boost-1.0.txt", "LICENSES/Lua-5.1.txt",
                     "LICENSES/MTA-Public-SDK-reference.txt"):
            (self.source / name).write_text("notice:" + name, encoding="utf-8")

    def test_complete_release_has_notices_and_correct_checksums(self):
        finalize(self.assets, self.source)
        with zipfile.ZipFile(self.assets / "ml_uuid-notices.zip") as package:
            self.assertEqual(set(package.namelist()), {
                "LICENSE", "THIRD_PARTY_NOTICES.md", "LICENSES/Boost-1.0.txt",
                "LICENSES/Lua-5.1.txt", "LICENSES/MTA-Public-SDK-reference.txt"})
            self.assertEqual(package.read("LICENSE"), b"notice:LICENSE")
        lines = (self.assets / "SHA256SUMS").read_text().splitlines()
        self.assertEqual(len(lines), 4)
        for line in lines:
            digest, name = line.split("  ")
            self.assertEqual(digest, hashlib.sha256((self.assets / name).read_bytes()).hexdigest())

    def test_missing_platform_blocks_packaging(self):
        (self.assets / BINARIES[1]).unlink()
        with self.assertRaises(ValueError):
            finalize(self.assets, self.source)
        self.assertFalse((self.assets / "SHA256SUMS").exists())

    def test_empty_binary_blocks_packaging(self):
        (self.assets / BINARIES[0]).write_bytes(b"")
        with self.assertRaises(ValueError):
            finalize(self.assets, self.source)

    def test_missing_license_blocks_packaging(self):
        (self.source / "LICENSES/Lua-5.1.txt").unlink()
        with self.assertRaises(ValueError):
            finalize(self.assets, self.source)

    def test_stage_handles_visual_studio_subdirectory_and_distinct_names(self):
        build = self.root / "build" / "src" / "Release"
        build.mkdir(parents=True)
        (build / "ml_uuid.dll").write_bytes(b"test-dll")
        for name in BINARIES[:2]:
            staged = stage(self.root / "build", name, self.root / "dist")
            self.assertEqual(staged.name, name)
            self.assertEqual(staged.read_bytes(), b"test-dll")

    def test_stage_rejects_missing_and_ambiguous_build_output(self):
        build = self.root / "build"
        build.mkdir()
        with self.assertRaises(ValueError):
            stage(build, BINARIES[0], self.root / "dist")
        for configuration in ("Debug", "Release"):
            directory = build / configuration
            directory.mkdir()
            (directory / "ml_uuid.dll").write_bytes(b"test-dll")
        with self.assertRaises(ValueError):
            stage(build, BINARIES[0], self.root / "dist")

    def test_linux_stage_preserves_binary(self):
        build = self.root / "build" / "src"
        build.mkdir(parents=True)
        (build / "ml_uuid.so").write_bytes(b"test-so")
        staged = stage(self.root / "build", BINARIES[2], self.root / "dist")
        self.assertEqual(staged.name, BINARIES[2])
        self.assertEqual(staged.read_bytes(), b"test-so")


if __name__ == "__main__":
    unittest.main()
