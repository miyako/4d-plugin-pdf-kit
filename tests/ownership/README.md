# PDF Kit ownership regression

Run `python3 tests/ownership/run.py [source-checkout]` on macOS with Xcode
command-line tools and Python 3.9 or later. No third-party test dependencies
are required. The source checkout defaults to the current directory; the
harness always comes from the directory containing the runner. This permits
the identical harness to test an immutable archive and the fixed sources.

The runner compiles complete source files, including the unchanged engine SDK,
without invoking either platform project. It records SHA-256 identities,
checks imports before execution, and substitutes synthetic PDF classes for
Quartz/PDFKit. The normal consumer fixtures and failed-resize probe run in
separate processes. Each process's exit status is printed; any failure makes
the runner exit nonzero. The failed-resize probe intentionally crashes under
ASan on the unfixed baseline. It is not an expected failure on fixed sources.

## Inventory and reachability

The complete native source inventory is the engine implementation
`PDF Kit/4D Plugin API/4DPluginAPI.c`, the four `Classes/*.cpp` wrappers,
`PDF Kit/4DPlugin-PDF-Kit.cpp`, and `PDF Kit/support/4DPlugin-JSON.cpp`.
The harness compiles all seven, with their actual headers. No source body or
SDK function is extracted or replaced; only host engine entry points and
external Apple boundaries are synthetic.

Xcode's Sources phase includes the SDK, dispatcher and all four wrappers.
It links Cocoa and PDFKit. The JSON helper is dormant: it is not a build
member and has no actual plugin consumers. Its defects are not exported
command crashes. The Windows project lists only the SDK and dispatcher,
omits wrapper membership, and cannot establish Windows compatibility for this
macOS PDFKit plugin. The two `templates/main.*` files contain 4D code-generation
directives, not additional executable command implementations. Bundled JSON
headers and static libraries are not used by the dispatcher or harness.
Tracked sample files, databases, packaged binaries and generated plugin
resources are not opened or executed.

The manifest has 12 selectors. Selectors 7 and 9 emit text arrays; selector 10
consumes independent snapshots of borrowed array strings. Selectors 2, 11 and
12 emit scalar text using the SDK's engine-backed content setter. The other
selectors manipulate in-memory blobs or accept a borrowed picture. Every
selector is executed by the real dispatcher against synthetic data. Annotation
fixtures cover text, free text, ink, line, link, stamp and generic branches;
the ink path list is empty. The page-label assignment bug is deliberately
preserved and asserted, not repaired as part of ownership work.

There are no plugin callbacks, retained callback variables, collection-element
consumers, field setters or direct method-result consumers beyond the dormant
JSON command helpers. Dormant path/URL helpers are classified but not executed:
their filesystem boundary functions throw, and NSURL initialization and common
external NSData constructors are denied before plugin execution. Successful
path-copy routines return independent CF/Objective-C owners, with constant
empty fallbacks on failure. Error behavior of those Apple APIs is not proven.

## Bundled SDK contract

The header's `v14` label is not an upstream release identity. The baseline
engine source and ABI blobs below remain unchanged:

| File in `PDF Kit/4D Plugin API/` | Git blob |
|---|---|
| `4DPluginAPI.c` | `62f745e91d81ef2c3c901c656ea8e175d3b173e8` |
| `4DPluginAPI.h` | `528bf1d2552df08d6a8b1bc95011a9b72934ad5c` |
| `PrivateTypes.h` | `a3098cb662cdbc43ba574fdef72a059c4c6d1cd4` |
| `PublicTypes.h` | `9ae20b001bff9128f690fff3e6c3f873b6238333` |
| `EntryPoints.h` | `da428d5f6ca23b0a30371c7288a01d29aa3c466a` |
| `Flags.h` | `7a5a35539f27430a3fd9cb711d6b789051316471` |

`PA_SetStringInArray` disposes the old element and shallow-transfers the input
only for Unicode array kind, non-null storage, non-null input and indices from
zero through the inclusive count. The caller retains rejected inputs.
`PA_GetStringInArray` returns a borrowed descriptor; input snapshots must copy
its content. Normal ARRAY_TEXT output already creates independent owners for
each element and output, without disposing transferred aliases; this is
preserved.

The scalar string and picture setters shallow-assign and do not clear a
previous owner. Initializing a variable with `PA_CreateVariable` and immediately
overwriting its allocated empty string/picture leaks that owner. Setters now
initialize plain descriptors instead. Existing consuming conventions for
picture/object/collection helpers are preserved; unknown external callers do
not justify inventing a borrowed contract.

SDK comments require successful property results to be cleared. Scalar and
mismatched results are now scoped owners; matching object/collection results
explicitly release that owner to the existing returned-reference consumer.
Getter, existence-command and stringify-command errors are captured before
kind inspection, adoption or cleanup. A raw type-zero failed output is
unspecified and can normalize to text, so kind checks alone are not guards.
SDK array getters and scalar variable-to-string results are treated as aliases,
not independent allocations.

The three pointer-owning wrappers have no named consumers that require copies.
Their implicit raw-pointer copy/assignment is disabled; CBytes contains a
value vector and C_LONGINT a scalar, so their normal value copying is unchanged.

## Same-harness evidence

Original production baseline: `0d0dd675bc45a241e86f203fc429022d3fca8209`.
Harness-only follow-up baseline: `c5e9deccfe291e918d8b7e96fc3a657196e6432a`,
which merged the harness commit `614b04b` without changing production sources.
The following results use the final, identical merged harness, not the earlier
206-assertion development revision.

| Process | Baseline | Fixed |
|---|---|---|
| All consumers | 208 assertions, 39 failures, exit 1 | 208 assertions, 0 failures, exit 0 |
| Isolated failed resize | ASan heap-buffer-overflow, process exit -6 | 2 assertions, 0 failures, exit 0 |
| Runner | exit 1; process codes `[1, -6]` | exit 0; process codes `[0, 0]` |

All normal dispatcher, unit-length, terminator, content, reuse and transfer
controls pass on both revisions. Exact baseline failure names and counts:

| Assertion | Failures |
|---|---:|
| `C_TEXT rejects shallow owner copies` | 1 |
| `ARRAY_TEXT rejects shallow owner copies` | 1 |
| `C_BLOB rejects shallow owner copies` | 1 |
| `narrow setter isolated owner balance` | 4 |
| `narrow setter/getter owner balance` | 4 |
| `wide-key setter owner balance` | 4 |
| `wide-value setter owner balance` | 1 |
| `native UTF16 setter/getter owner balance` | 1 |
| `picture setter independent owner balance` | 1 |
| `all six getters success/mismatch owner balance` | 4 |
| `failed command reported before result adoption` | 2 |
| `failed property-existence command reported` | 7 |
| `failed property reported before result adoption` | 6 |
| `null array storage reported before allocating output` | 1 |
| `null array storage rejected output owner balance` | 1 |

The isolated ASan failure is in actual SDK `PA_SetStringInArray`, line 4945,
after the wrapper continues past a failed growth. The fix captures the resize
error immediately and throws before insertion or publishing its local count.
The fixture proves the caller's original metadata and owners remain intact on
this failed growth path. It does not infer physical capacity from metadata or
from a size reread.

Harness SHA-256:

| File | SHA-256 |
|---|---|
| `run.py` | `0b78bdcd75109702217c1948f1a5ff8b2292c06cb9ae763fe9ba29a95f523871` |
| `main.mm` | `5c858860ea6c5e60e41296219476713950cd96b901a583cdb4e79076bc7c7d70` |
| `Quartz/Quartz.h` | `5430c40113aef6a09dab16b3a98d0d6af9c5762a1212d836299a01cc771c2604` |

Retained local evidence logs: `merged-harness-baseline.log` SHA-256
`0b9f5c27dc12b636b6e0517badbcb9b12df1b693dd41972cb62fc55fb3cd4303`;
`fixed.log` SHA-256
`bc7d367ef64b5984528efb781b593826cecdfcc92ea1bc674a1776aa92845bd2`.
These logs include exact compiled source hashes. They are retained outside
the checkout; CI independently prints provenance and results.

## Boundaries and precise deferrals

The test uses ASCII, BMP and supplementary text, allocated empty strings,
UTF-16 code-unit lengths, terminators, repeated output replacement, independent
arrays, reserved index zero, borrowed snapshots, rejection controls and
ownership balances. CoreFoundation conversions run in memory with their real
byte/unit contracts. No user PDFs, database contents, network/device/UI/settings
operations or actual PDFKit/graphics operations execute. The picture dispatcher
boundary returns null, so it proves the borrowed 4D picture survives that branch,
not the lifetime of a real CGImage or the successful image-rendering branch.
Only explicitly handled host selectors are accepted.

ASan and UBSan are enabled except packed SDK alignment instrumentation.
macOS does not provide LSan here; explicit shim balances cover tracked string,
handle, picture and reference owners, not arbitrary Apple allocations.
No native Windows, real 4D host, cross-thread callback, actual Apple path-error,
or real allocator exhaustion result is claimed.

The engine's resize implementation updates its local count after failure and
can dispose truncated owners before a failed shrink. That engine behavior,
damaged metadata, allocation failure during SDK variable/string creation,
and unspecified partially initialized failure outputs remain deferred. The
wrapper guard prevents demonstrated post-failure insertion, not every possible
OOM cleanup hazard. Unknown external helper-consuming contracts and host
content-setter retention cannot be resolved by a convenient shim assumption.

Before validation, all project phases, templates, CI triggers and repository
hook metadata were inspected. Xcode Deployment signs, packages and submits
notarization; template phases can also sign. None is invoked. The workflow
uses only read-only checkout and this unsigned harness on `macos-15`, on PRs
targeting master and pushes to master. No tags, releases, artifact publishing,
manual dispatch or release-project builds are involved.
