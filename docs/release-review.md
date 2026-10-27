# Release readiness review

Updated on 2026-10-27 after reviewing main at
`ae4b1dd772f42c255f9c3a216e2b8ccd5673a15c` and merged PR #5.

## Supported targets

Windows x86, Windows x64 and Linux x64. Linux x86 is outside the current
support scope and is not a missing release target.

## Verified baseline

[Main CI run](https://github.com/psychoo102/ml_uuid/actions/runs/40178886852)
passed on all three targets. Its logs confirm:
- successful native compilation;
- the native UUID callback regression executable passed;
- all seven release-packaging tests passed;
- distinctly named platform binaries were staged and uploaded.

The callback tests cover UUID format, version, variant, v7 timestamp sanity,
sample uniqueness, concurrent calls, persistent generator state across Lua
states, null states and injected generation failures. They use a Lua test
double and do not establish compatibility with a running MTA process.

The owner reports that the module loads and works on Windows and Linux MTA
servers. Exact server versions, architectures and tested binary revisions
were not recorded. Treat this as manual confirmation for the tested setups;
do not infer coverage of every supported target or restart/reload scenario.

## Changes in this follow-up

The Linux importer now owns its temporary `dlopen(RTLD_NOLOAD)` reference
with RAII. The reference is released on successful symbol resolution and
on every missing-symbol return. The host keeps ownership of the running
`deathmatch.so`; the importer does not load a second Lua implementation.

A Linux-only CTest exercises the production importer with wrapped dynamic
loader calls. It checks successful imports, failure to acquire the core,
missing symbols at early/middle/late import stages, and repeated attempts.
This is a loader lifetime regression test, not a real MTA integration test.
See this PR's CI results for validation of the follow-up itself; the baseline
run above predates this change.

## Remaining follow-up

### SDK terms and provenance

The current upstream Public SDK license excludes commercial applications.
Project MIT licensing does not replace third-party terms. Exact historical
SDK provenance and applicability to commercial/premium servers remain
unresolved; see [SDK provenance](sdk-provenance.md) and
[third-party notices](../THIRD_PARTY_NOTICES.md).
Obtain clarification from the rights holder before claiming unrestricted
commercial reuse. No such permission is asserted by this review.

### Runtime compatibility record

Record OS, architecture, MTA version, module commit/release and test results
for each manually tested setup. Include resource restarts and module reloads
when available. Inspect actual binary dependencies before expanding the
Linux compatibility claim: a build on the CI runner alone does not establish
compatibility with older glibc/libstdc++ installations.

The importer still uses core paths relative to the process working directory
and requires the legacy Lua symbol set. Custom layouts may fail cleanly even
when the UUID callbacks themselves need only a few of these symbols.

### Published release versus main

The current `v1.1.3` tag points to
`7668fda84ac0e5f0b31424955cf427b808d02ee3`, before PR #5.
That PR added tests and provenance documentation without changing the UUID
callback implementation. A future release should include this follow-up and
its successful CI results. This review does not retag or replace old assets.

### Optional maintenance

- Remove unused CLuaArgument/CLuaArguments helpers and legacy root UUID
  implementations after checking consumers; they are not used by callbacks.
- CMake is the maintained build entry point; legacy Visual Studio project
  files are not validated as an alternative release pipeline.
- Check registration return values and improve partial-registration logging.
- The MTA float version remains `1.1`; package/release metadata carries the
  full version.
- Update the vcpkg binary-cache setup, whose removed `x-gha` backend currently
  emits a warning without failing these builds.
