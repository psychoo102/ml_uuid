# Third-party notices and release blocker

The top-level MIT license applies to original ml_uuid contributions.
It does not replace third-party licenses.

## Boost.UUID

The active generator uses Boost.UUID from vcpkg (the pinned baseline currently
selects Boost 1.91.0). The root-level legacy `uuid.hpp` is also a Boost file.
See [Boost Software License](LICENSES/Boost-1.0.txt).
The root-level `uuid.h` is a separate unused implementation; identify its
upstream provenance before retaining it in a release source package.

## Lua 5.1

The bundled Lua headers carry the Lua.org / PUC-Rio notice from 1994-2008.
See [Lua notice](LICENSES/Lua-5.1.txt) and the notice in `include/lua.h`.
The origin and build recipe for the checked-in `lib/lua5.1.lib` and
`lib/x64/lua5.1.lib` should also be recorded.

## Lua import shim

`luaimports/luaimports.cpp` and `luaimports/luaimports.h` originate from
[mta-mysql's Lua importer](https://github.com/multitheftauto/mtasa-modules/tree/master/mta-mysql/luaimports),
part of the [MTA modules collection](https://github.com/multitheftauto/mtasa-modules).
They carry Alberto Alonso's 2008 BSD-style notice. The copy in this module has
been adapted (including diagnostic text). Retain those full notices in source distributions and
reproduce them with binary packages. The full notice is reproduced below.

Copyright (c) 2008, Alberto Alonso

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this
      list of conditions and the following disclaimer in the documentation and/or other
      materials provided with the distribution.
    * Neither the name of the mta-mysql nor the names of its contributors may be used
      to endorse or promote products derived from this software without specific prior
      written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 

## MTA Public SDK: upstream identified, commercial scope unresolved

The bundled `ml_base`-derived files (`src/ml_uuid.cpp`, `include/ml_uuid.h`,
`include/Common.h`), `include/ILuaModuleManager.h` and the legacy
`CLuaArgument`/`CLuaArguments` helpers trace to the
[Multi Theft Auto Public SDK](https://github.com/multitheftauto/mtasa-blue/tree/master/Shared/publicsdk).
The template and helpers were adapted for this project. The exact historical
SDK commit used for the original copy is not established.

The separately licensed upstream
[Public SDK LICENSE](https://github.com/multitheftauto/mtasa-blue/blob/master/Shared/publicsdk/LICENSE)
explicitly excludes commercial applications. Its text is included at
[LICENSES/MTA-Public-SDK-reference.txt](LICENSES/MTA-Public-SDK-reference.txt).
The top-level MTA repository's GPLv3 does **not** by itself supersede this
folder-specific license. Redistributing the code under the project's MIT file
cannot remove upstream restrictions. The scope of permitted use in premium or
otherwise commercial servers has not been confirmed by the rights holder.

See [docs/sdk-provenance.md](docs/sdk-provenance.md) for file-by-file links and
remaining questions. Before advertising unrestricted commercial reuse, obtain
authoritative clarification, confirm applicable versions and preserve source
notices; if necessary, seek legal advice. Retain the required notices when
redistributing the library.
