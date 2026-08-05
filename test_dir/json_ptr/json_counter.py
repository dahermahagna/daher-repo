import json
import argparse
from pathlib import Path

def count_nodes(json_obj):
    """Recursively counts the number of nodes. Containers count as 3, others as 1."""
    count = 0

    if isinstance(json_obj, dict):
        count += 3  # Dictionary container counts as 3
        # print(f"Counting dict item")
        for key, value in json_obj.items():
            count += count_nodes(value)

    elif isinstance(json_obj, list):
        count += 3  # List container counts as 3
        # print(f"Counting list item")
        for item in json_obj:
            count += count_nodes(item)
    else:
        # print(f"Counting leaf {json_obj}")
        count += 1  # Final value (leaf) counts as 1
    return count

def find_subtree(json_obj, target_key):
    """Searches for a subtree by the specified key."""
    if isinstance(json_obj, dict):
        if target_key in json_obj:
            return json_obj[target_key]
        for key, value in json_obj.items():
            result = find_subtree(value, target_key)
            if result is not None:
                return result
    elif isinstance(json_obj, list):
        for item in json_obj:
            result = find_subtree(item, target_key)
            if result is not None:
                return result
    return None

def count_nodes_in_subtree(json_obj, target_key):
    """Finds a subtree and counts its nodes using the new weight."""
    subtree = find_subtree(json_obj, target_key)
    if subtree is None:
        return 0
    return count_nodes(subtree)


def iter_json_files(root_dir: Path):
    skip_dirs = {".git", ".venv", "venv", "__pycache__"}
    files = []

    for path in root_dir.rglob("*.json"):
        if not path.is_file():
            continue

        rel_parts = path.relative_to(root_dir).parts
        if any(part in skip_dirs or part.startswith(".") for part in rel_parts[:-1]):
            continue

        files.append(path)

    return sorted(files)


def main():
    parser = argparse.ArgumentParser(description="Count weighted nodes for all JSON files.")
    parser.add_argument(
        "--root",
        default=".",
        help="Root directory to scan recursively for .json files (default: current directory).",
    )
    args = parser.parse_args()

    root_dir = Path(args.root).expanduser().resolve()
    if not root_dir.exists() or not root_dir.is_dir():
        print(f"Invalid root directory: {root_dir}")
        return

    json_files = iter_json_files(root_dir)
    if not json_files:
        print(f"No JSON files found under: {root_dir}")
        return

    print(f"Scanning {len(json_files)} JSON files under: {root_dir}")
    for json_file in json_files:
        rel_path = json_file.relative_to(root_dir)
        try:
            with open(json_file, "r", encoding="utf-8") as file:
                data = json.load(file)
            if rel_path.name == "data-ds-rf-port.json":
                for key in ["ds-rf-port"]:
                    subtree = find_subtree(data, key)
                    if isinstance(subtree, list):
                        for i, entry in enumerate(subtree, 1):
                            entry_count = count_nodes(entry)
                            entry_index = entry.get("index", f"item-{i}") if isinstance(entry, dict) else f"item-{i}"
                            print(f"{rel_path} {key}[{i}] index={entry_index}: {entry_count}")
                    else:
                        count = count_nodes(subtree) if subtree is not None else 0
                        print(f"{rel_path} {key}: {count}")
            if rel_path.name == "data-mac-domain.json":
                for key in ["mac-domain"]:
                    subtree = find_subtree(data, key)
                    if isinstance(subtree, list):
                        for i, entry in enumerate(subtree, 1):
                            entry_count = count_nodes(entry)
                            entry_index = entry.get("index", f"item-{i}") if isinstance(entry, dict) else f"item-{i}"
                            print(f"{rel_path} {key}[{i}] index={entry_index}: {entry_count}")
                    else:
                        count = count_nodes(subtree) if subtree is not None else 0
                        print(f"{rel_path} {key}: {count}")
                # print(f"{rel_path} mac-domain: {count_nodes_in_subtree(data, 'mac-domain')}")
            # print(f"subtree {rel_path} mac-domain: {count_nodes_in_subtree(data, 'mac-domain')}")
            print(f"{rel_path}: {count_nodes(data)}")
        except Exception as err:
            print(f"{rel_path}: ERROR ({err})")


if __name__ == "__main__":
    main()