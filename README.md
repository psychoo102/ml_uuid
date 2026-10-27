# ml_uuid

Server-side UUID v4 and v7 generation for Multi Theft Auto: San Andreas.
The module exposes two Lua functions backed by Boost.UUID.

## Lua API

| Function | Success | Failure |
| --- | --- | --- |
| `uuidv4()` | A lowercase, 36-character UUID v4 string | `nil, "UUID generation failed"` |
| `uuidv7()` | A lowercase, 36-character UUID v7 string | `nil, "UUID generation failed"` |

Both functions take no arguments. Extra arguments are ignored. These are
server-side module functions, available in resources after the module loads.

```lua
local vehicleId, err = uuidv7()
if not vehicleId then
    outputDebugString(err, 1)
    return
end
dbExec(connection, "INSERT INTO vehicles (id) VALUES (?)", vehicleId)
```

UUIDs are identifiers. Generation does not check your database; enforce
uniqueness with a primary key or UNIQUE constraint.

### UUID v4 and v7

- **v4** uses Boost's random generator, seeded from the platform entropy source.
- **v7** embeds the Unix timestamp in milliseconds and is suitable for
  time-oriented identifiers. It reveals approximate creation time.
- Generator state is retained across calls and resource Lua VMs within one
  loaded module. Access to each generator is synchronized.
- v7 is not a global sequence across servers, module reloads, or restarts.
  Boost's monotonicity guarantee requires a non-decreasing system clock,
  at least millisecond clock resolution, and no more than 64 UUIDs per
  microsecond. Do not use UUID order as a substitute for business event order.
- This project does not claim that UUIDs are authentication secrets.

Reference: [Boost.UUID](https://www.boost.org/doc/libs/latest/libs/uuid/doc/html/uuid.html)
and [RFC 9562](https://www.rfc-editor.org/rfc/rfc9562.html).

## Installation

Download the [current v1.1.3 release](https://github.com/psychoo102/ml_uuid/releases/tag/v1.1.3).
Choose the binary matching the **server process architecture**, not just the OS.

| Server platform | Release asset | Installed filename | Server module directory |
| --- | --- | --- | --- |
| Windows x86 | `ml_uuid-win32.dll` | `ml_uuid.dll` | `mods/deathmatch/modules/` |
| Windows x64 | `ml_uuid-win64.dll` | `ml_uuid.dll` | `x64/modules/` |
| Linux x64 | `ml_uuid-linux-x64.so` | `ml_uuid.so` | `x64/modules/` |

These directories follow the traditional MTA server layout; verify against
any custom distribution. Rename the downloaded binary as above, place it in
the matching modules directory, and add to `mtaserver.conf`:

```xml
<module src="ml_uuid.dll" />
```

On Linux use `<module src="ml_uuid.so" />`.
Restart the server, inspect the module load log, and run the smoke resource below.

If loading fails, check architecture, configured filename, dependent libraries,
and the working directory. The Linux importer currently looks for
`x64/deathmatch.so` on x64 and `mods/deathmatch/deathmatch.so` on x86,
relative to the process working directory. The owner has confirmed loading and operation on Windows and Linux servers.
Exact MTA versions and architectures for those manual tests were not recorded;
this is not a claim that every distribution has been tested. The importer
releases its temporary core-library reference after each import attempt.

## Building

Requirements:

- C++17 compiler and Boost.UUID **1.86 or newer**.
- CMake 3.21+ for presets. The supplied Windows presets select
  **Visual Studio 2026**, which requires **CMake 4.2+** and the C++ workload.
- Ninja and a C/C++ toolchain for the Linux presets.
- An installed and bootstrapped vcpkg checkout; set `VCPKG_ROOT` to its path.
  Presets use this checkout. The repository does not install vcpkg automatically.

Windows x86:

```powershell
cmake --preset win32-release
cmake --build --preset win32-release
```

For Windows x64 use `win64-release`. Windows build presets explicitly select
Release and use the static MSVC runtime to match the vcpkg static triplets.

Linux x64:

```sh
cmake --preset linux-x64-release
cmake --build --preset linux-x64-release
```

Output is under `build/<preset>/src/` (plus `Release/` for Visual Studio).
Use CMake as the maintained build entry point. The legacy Visual Studio project
files remain in the repository but are not validated by this preparation.

Boost is header-only here; that does not eliminate system runtime dependencies.
Check the actual artifact with `ldd`/`readelf` on Linux or
`dumpbin /dependents` on Windows before distributing it.

## Verification

Native tests compile the production UUID callback implementation against a
minimal Lua API test double, without starting MTA. They exercise UUID v4/v7
format and variant, timestamp sanity, sample uniqueness, retained generator
state across Lua VMs, concurrent calls, null VM handling, and the C++
exception-to-`nil, error` path. They run with CTest in CI for each build target.

For a local configured build, run:

```sh
ctest --test-dir build/linux-x64-release --output-on-failure
```

Use the matching build preset on Windows and add `--build-config Release` for
Visual Studio generators. These tests do **not** prove ABI compatibility with
a running MTA process.

Copy `tests/mta/uuid_smoke` into your server resources and start
`uuid_smoke` after loading the module. It checks API availability, string format,
version and variant bits, sample uniqueness, and the v7 timestamp.

The test reports ordering regressions separately: ordering is conditional,
and a clock adjustment can cause one. Sample uniqueness is a regression check,
not proof that collisions are impossible. Repeat on each supported platform,
including resource restarts and module reloads.

See [release preparation review](docs/release-review.md) for outstanding blockers.
See [release workflow](docs/releasing.md) for tag-triggered binary publication.

## Contributing

Include your OS, server architecture and version, module version, reproduction
steps, relevant logs, and whether you used a release binary or built from source.
Changes to generation or loading should be tested on a real MTA server.

## Licensing

Original project contributions are covered by the top-level MIT license.
Bundled third-party code retains its own notices and terms; the complete
distribution must not be described as unconditionally MIT or commercially
permissive until the MTA SDK provenance and license are resolved.

See [third-party notices](THIRD_PARTY_NOTICES.md) and the
[SDK provenance review](docs/sdk-provenance.md). The MTA Public SDK excludes
commercial applications under its own license; do not advertise the entire
module as unconditionally MIT-licensed or commercially unrestricted. Resolve
the applicability of that restriction before making such a claim.

## Release assets

The current release supplies binaries for Windows x86, Windows x64 and Linux
x64, a `SHA256SUMS` checksum file and `ml_uuid-notices.zip`. Rename the chosen
binary to the installed filename in the table above. Keep the notices when
redistributing binaries and verify the SHA-256 checksum for your download.

The v1.1.3 CI build passed, but testing the released files inside real MTA
servers remains a separate compatibility step.
