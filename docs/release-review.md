# Public release preparation review

Reviewed main at `a4bb7ed6a78c80f65ad66c7ed7b9bad88baba4d6` on 2026-10-10.
This is a source review and preparation branch, not a certified binary release.
The GitHub connection supplied sources after direct git clone failed.
No existing release binary was downloaded or executed.

## Fixed in this preparation

| Finding | Change |
| --- | --- |
| New Boost generators were constructed and seeded on every Lua call; v7 conflict state was discarded | Reuse generators with synchronized access |
| C++ generation/formatting exceptions could escape Lua callbacks | Return nil plus a stable error string |
| A live std::string surrounded a Lua allocation call that can longjmp | Destroy non-trivial automatic objects before pushing to Lua |
| Windows build presets defaulted to Debug even when named release | Set build preset configuration explicitly |
| Linux debug/release shared one build directory | Separate directories |
| vcpkg fallback selected after project() | Select before compiler initialization |
| Boost version requirement unspecified despite v7 needing 1.86+ | Require Boost 1.86+ |
| Static vcpkg CRT could differ from module CRT | Set matching MSVC runtime in Windows presets |
| Linux linking suppressed all unresolved-symbol errors | Remove blanket suppression |
| Manifest, CMake and module display versions diverged | Manifest now matches 1.1.2; MTA float API still displays 1.1 |
| Initialization flag survived shutdown/reinitialization | Reset lifecycle state |
| README omitted release asset renaming, architecture and failure behavior | Document these, build prerequisites and ordering limitations |
| Third-party terms were obscured by blanket MIT claim | Add notices and flag unresolved SDK provenance |
| Editor state was tracked | Remove .aps and .vcxproj.user; ignore future copies |

## Remaining release blockers

### 1. SDK licensing and provenance (high priority)

See THIRD_PARTY_NOTICES.md. Identify the exact original SDK/template revision.
The upstream Public SDK reference excludes commercial applications; this cannot
be overwritten by a project MIT file. Do not infer that premium servers are
authorized until the applicable terms have been established.

### 2. Build, ABI and real server loading (high priority)

Build Windows x86, Windows x64 and Linux x64. Test in the specific MTA releases
you intend to support. Verify exported entry points, process architecture,
Lua imports and actual runtime dependencies.
Use dumpbin /exports and /dependents on Windows, and nm -D, readelf and ldd
on Linux. Do not link a different Lua VM simply to make compilation succeed.

The existing Linux importer uses hard-coded relative deathmatch.so paths,
requires many Lua symbols this module does not use, and never closes its dlopen
reference (including failure paths). This needs integration-tested cleanup.
Replacing lookup with a generic global dlsym can accidentally resolve the wrong
Lua implementation; do not do so without checking the host loader behavior.

### 3. Tests and CI (high priority, next stage)

There were no tests in the reviewed tree. The new MTA smoke resource covers
format, version, variant, sample collisions and timestamp extraction. Run it
on all targets and after restart/reload; report ordering separately because
clock behavior matters.

Also add executable native regression tests for callback exception handling
and persistent v7 state, preferably with a controllable clock/entropy source.
These cannot be proven by inspecting code or a sample uniqueness test.

Existing CI is intentionally unchanged. Findings for the next stage:
- Verify runner CMake/VS availability for VS 2026 (requires CMake 4.2+).
- Record and pin the vcpkg checkout; the manifest baseline alone does not pin the tool.
- Two Windows jobs upload the same ml_uuid.dll asset name to one release.
- A Linux artifact path glob is not proof that the module can load.
- if-no-files-found: warn permits green jobs without a binary.
- The release condition mentions a release event absent from the workflow triggers.
- Release upload depends on a custom TOKEN secret; review minimal permissions
  and use of GITHUB_TOKEN.
- Publish only after every build/test succeeds, with distinct platform names,
  checksums, notices and documented compatibility.
- Do not let a partial matrix publish an apparently complete stable release.

### 4. Existing releases (medium priority)

v1.1.2 has empty release notes. Its Linux SO has the same SHA-256 digest as
v1.1.1; this is observable asset metadata, not proof of corruption.
Record whether reuse was intentional, and document which source revision and
toolchain produced each artifact. The changes in this branch do not alter
already published files.

### 5. Legacy source and projects (medium priority)

Unused root uuid.h and uuid.hpp remain. The Visual Studio project still lists
both and has separate toolset/runtime settings; CMake is documented as the
maintained entry point. Remove or reconcile legacy build files after platform
validation rather than claiming both paths are supported.

CLuaArgument and CLuaArguments are compiled by the source glob but unused by
the UUID callbacks:
- String self-assignment clears the source before copying.
- Argument-vector self-assignment empties its own contents.
- Assignment operators return the source rather than *this.
- Lua string copying uses strlen and loses embedded NUL bytes.
- Call handles only some lua_pcall failures and leaves error values on the stack.
Prefer removing these helpers from this module after checking that they have
no consumers. These are latent helper bugs, not current uuidv4/v7 exploits.

### 6. Module initialization diagnostics (medium priority)

RegisterFunction return values are ignored. Failure could leave only part of
the API registered. Add useful logs and integration tests for registration
failure. Bound metadata copies are safe for the present short constants, but
future longer names need explicit termination. The MTA float version field
cannot represent semantic version 1.1.2; use release/package metadata for it.

## Validation status

- Reviewed core callbacks, module lifecycle, CMake, presets, manifest,
  legacy argument helpers, Lua import code, README, license notices and release metadata.
- CMake --list-presets=all executed successfully using the installed VS-bundled CMake.
- JSON structure and change scope checked before creating the draft PR.
- Native compilation, linked-artifact inspection, failure injection and MTA
  smoke tests have NOT been run in this task.
- CI workflow was not modified.
- Do not merge/publish as a fully validated release until the blockers above
  are closed.
