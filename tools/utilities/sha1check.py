#!/usr/bin/env python3
"""sha1check.py FILE EXPECTED_SHA1 -- exit 0 only on exact match."""
import hashlib
import sys

h = hashlib.sha1(open(sys.argv[1], "rb").read()).hexdigest()
ok = h == sys.argv[2].lower()
print(f"{sys.argv[1]}: {h} {'OK' if ok else 'MISMATCH (expected ' + sys.argv[2] + ')'}")
sys.exit(0 if ok else 1)
