#!/usr/bin/env python3
"""Build complete actual sources, never the signing/packaging project."""
import hashlib
import os
from pathlib import Path
import subprocess
import sys
import tempfile

root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
here = Path(__file__).resolve().parent
plugin = root / "PDF Kit"
sources = [
    "4D Plugin API/4DPluginAPI.c",
    "4D Plugin API/Classes/C_TEXT.cpp",
    "4D Plugin API/Classes/ARRAY_TEXT.cpp",
    "4D Plugin API/Classes/C_BLOB.cpp",
    "4D Plugin API/Classes/C_LONGINT.cpp",
    "support/4DPlugin-JSON.cpp",
    "4DPlugin-PDF-Kit.cpp",
]
for name in sources + [
    "4D Plugin API/4DPluginAPI.h", "4D Plugin API/PrivateTypes.h",
    "4D Plugin API/PublicTypes.h", "4D Plugin API/EntryPoints.h",
    "4D Plugin API/Flags.h", "4D Plugin API/Classes/C_TEXT.h",
    "4D Plugin API/Classes/ARRAY_TEXT.h", "4D Plugin API/Classes/C_BLOB.h",
    "4D Plugin API/Classes/C_TYPES.h", "manifest.json",
]:
    print(hashlib.sha256((plugin / name).read_bytes()).hexdigest(), name, flush=True)
for name in ("run.py", "main.mm", "Quartz/Quartz.h"):
    print(hashlib.sha256((here / name).read_bytes()).hexdigest(), name, flush=True)

with tempfile.TemporaryDirectory(prefix="pdf-kit-ownership-") as directory:
    out = Path(directory)
    flags = [
        "-g", "-O1", "-fsanitize=address,undefined", "-fno-sanitize=alignment",
        "-fno-omit-frame-pointer", "-Wno-deprecated-declarations", "-Wno-multichar",
        "-Wno-deprecated-builtins", "-I" + str(here),
        "-I" + str(plugin / "4D Plugin API"),
        "-I" + str(plugin / "4D Plugin API/Classes"),
        "-I" + str(plugin / "support"),
    ]
    objects = []
    for i, source in enumerate([plugin / n for n in sources] + [here / "main.mm"]):
        obj = out / (str(i) + ".o")
        language = ["-x", "c", "-Wno-int-conversion"] if source.suffix == ".c" else [
            "-x", "objective-c++", "-std=c++14"
        ]
        subprocess.run(["clang++", *flags, *language, "-c", str(source), "-o", str(obj)],
                       check=True)
        objects.append(str(obj))
    exe = out / "ownership"
    subprocess.run(["clang++", *flags, *objects, "-framework", "Cocoa",
                    "-framework", "CoreFoundation", "-o", str(exe)], check=True)
    imports = subprocess.check_output(["nm", "-u", str(exe)], text=True)
    exact = {
        "_abort", "_calloc", "_free", "_malloc", "_realloc", "_memcmp", "_memcpy",
        "_memmove", "_memset", "_strlen", "_wcslen", "_isspace", "_sprintf",
        "_snprintf", "_vsnprintf", "_printf", "_fprintf", "_fwrite", "_puts",
        "_exit", "_atexit", "_bzero", "dyld_stub_binder", "__Unwind_Resume",
        "__tlv_bootstrap", "_memchr", "_kCFAllocatorDefault", "_CFRelease",
        "_CFStringCreateWithBytes", "_CFStringCreateWithCharacters", "_CFStringGetLength",
        "_CFStringGetCharacters", "_CFStringGetMaximumSizeForEncoding", "_CFStringGetBytes",
        "_NSZeroSize", "_NSZeroPoint", "_objc_getClass", "_objc_getMetaClass",
        "_class_getInstanceMethod", "_class_getClassMethod", "_method_setImplementation",
        "_sel_registerName", "__DefaultRuneLocale", "__objc_empty_cache",
    }
    allowed_classes = {
        "NSObject", "NSString", "NSMutableString", "NSData", "NSArray",
        "NSDictionary", "NSMutableDictionary", "NSDate", "NSNull", "NSAutoreleasePool",
        "NSConstantArray", "NSURL",
    }
    symbols = [s.strip() for s in imports.splitlines() if s.strip()]
    unexpected = []
    for s in symbols:
        if any(p in s for p in ("basic_filebuf", "basic_ifstream", "basic_ofstream",
                                 "basic_fstream", "filesystem", "random_device")):
            raise RuntimeError("Forbidden external-data C++ import: " + s)
        if s.startswith("_OBJC_CLASS_$_") and s.removeprefix("_OBJC_CLASS_$_") in allowed_classes:
            continue
        if s in exact or s.startswith(("___", "__Z", "_objc_", "_OBJC_METACLASS_$_NSObject")):
            continue
        unexpected.append(s)
    if unexpected:
        raise RuntimeError("Unexpected imports before execution: " + repr(unexpected))
    linked = subprocess.check_output(["otool", "-L", str(exe)], text=True)
    if "Quartz.framework" in linked or "PDFKit.framework" in linked:
        raise RuntimeError("Real PDFKit must not be linked")
    print("PASS pre-execution imports and synthetic PDF boundary gate", flush=True)
    env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0", UBSAN_OPTIONS="halt_on_error=1")
    results = []
    for args in ([], ["--resize-failure"]):
        print("CASE", args or ["all-consumers"], flush=True)
        results.append(subprocess.run([str(exe), *args], env=env).returncode)
    print("case_exit_codes=" + repr(results), flush=True)
    sys.exit(1 if any(results) else 0)
