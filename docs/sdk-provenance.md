# Upstream provenance and license review

Reviewed for ml_uuid 1.1.3. The code origins below were found by comparing
names, headers and implementations with the original public repositories.
Identifying a repository and file does **not** prove which historical commit
was originally copied or confer permissions beyond the corresponding license.

## MTA Public SDK

Upstream tree:
https://github.com/multitheftauto/mtasa-blue/tree/master/Shared/publicsdk

| Files in ml_uuid | Corresponding upstream sources |
| --- | --- |
| `src/ml_uuid.cpp`, `include/ml_uuid.h` | `Shared/publicsdk/ml_base.cpp`, `Shared/publicsdk/ml_base.h` (adapted module template) |
| `include/Common.h` | `Shared/publicsdk/Common.h` (adapted) |
| `include/ILuaModuleManager.h` | `Shared/publicsdk/include/ILuaModuleManager.h` (older interface copy) |
| `src/CLuaArgument.cpp`, `include/CLuaArgument.h` | `Shared/publicsdk/extra/CLuaArgument.cpp`, `.h` |
| `src/CLuaArguments.cpp`, `include/CLuaArguments.h` | `Shared/publicsdk/extra/CLuaArguments.cpp`, `.h` |

The source headers themselves identify their origin as `ml_base` or the
Public SDK. The current upstream interface header contains additional methods
not present in the repository's older copy, so the latter should **not** be
advertised as a precise mirror of the current SDK.

Upstream folder-specific license:
https://github.com/multitheftauto/mtasa-blue/blob/master/Shared/publicsdk/LICENSE

The license explicitly excludes *commercial applications*. Its other terms
require proper origin identification, marking altered sources, and retaining
the license notice. The MTA repository root advertises GPLv3 by default
unless otherwise specified; this folder has its own specified license. The
repository's MIT license covers only original contributions, not this SDK.

**Unresolved:** locate the precise historical revision, determine how the SDK
restriction applies to a module distributed for use by commercial/premium
servers, and obtain clarification or permission from the rights holder before
asserting unrestricted commercial use. This document is not legal advice.

## Lua import shim

The Linux importer is derived from Alberto Alonso's `mta-mysql` project:
https://github.com/multitheftauto/mtasa-modules/tree/master/mta-mysql/luaimports

The original BSD-style license is available at:
https://github.com/multitheftauto/mtasa-modules/blob/master/mta-mysql/LICENSE

The author and redistribution terms are already reproduced in
`THIRD_PARTY_NOTICES.md`, with copyright retained in the sources. The importer
has been changed for ml_uuid (module log messages and integration choices).
Attribution and reproduction of the full notice must remain with binary
redistributions. This license does not itself exclude commercial use.

## Other bundled inputs

- `include/lua*.h`, `include/lauxlib.h`, `include/lualib.h`:
  Lua 5.1, MIT-style license reproduced in `LICENSES/Lua-5.1.txt`.
- Boost.UUID and legacy `uuid.hpp`: Boost Software License 1.0
  (`LICENSES/Boost-1.0.txt`).
- Legacy root-level `uuid.h`: exact upstream origin remains unverified.
- `lib/lua5.1.lib`, `lib/x64/lua5.1.lib`: binary provenance and build
  configuration remain unverified.

## Required release follow-up

1. Ask MTA maintainers for the interpretation/permission relevant to public,
   potentially commercial module distribution.
2. Remove unused copied template helpers and legacy implementations when safe,
   minimizing the footprint of unneeded third-party code.
3. Preserve license and original copyright notices in distributed archives.
4. Confirm the provenance/build recipe for both checked-in Lua import libraries.
5. Do not describe the **entire combined module** as permissively MIT licensed
   until the third-party obligations and commercial restrictions are addressed.
