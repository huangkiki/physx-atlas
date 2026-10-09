"""Verify pinned PhysX source blobs and Markdown source-link line bounds.

Reads an existing official source checkout/cache; does not download or run PhysX.
Semantic claims must still be reviewed against the referenced source content.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-root", type=Path, required=True,
                        help="Pinned upstream root containing physx/include")
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    manifest = json.loads((repo / "docs/sources.json").read_text())
    records = {item["path"]: item["git_blob"] for item in manifest["files"]}
    if len(records) != len(manifest["files"]):
        raise SystemExit("Duplicate source paths in manifest")
    line_counts = {}
    for path, expected in records.items():
        data = (args.source_root / path).read_bytes()
        blob = hashlib.sha1(b"blob " + str(len(data)).encode() + b"\0" + data).hexdigest()
        if blob != expected:
            raise SystemExit(f"Source blob mismatch: {path}")
        line_counts[path] = len(data.splitlines())
    pattern = re.compile(
        r"https://github.com/NVIDIA-Omniverse/PhysX/blob/([^/]+)/"
        r"([^\s)#]+)(?:#L(\d+)(?:-L(\d+))?)?"
    )
    links = 0
    for page in repo.rglob("*.md"):
        if ".git" in page.relative_to(repo).parts:
            continue
        for commit, path, start, end in pattern.findall(page.read_text()):
            if commit != manifest["commit"] or path not in records:
                raise SystemExit(f"Unregistered or unpinned source: {page.name}: {path}")
            if start and not 1 <= int(start) <= int(end or start) <= line_counts[path]:
                raise SystemExit(f"Invalid source line range: {page.name}: {path}")
            links += 1
    print(f"{len(records)} pinned source blobs and {links} source links passed.")
    print("Line existence and file identity do not verify semantic claims or runtime behavior.")


if __name__ == "__main__":
    main()
