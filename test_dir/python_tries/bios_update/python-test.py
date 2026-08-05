#open suggested_plan_file.json
import json
required_keys = [
            "from_version",
            "to_version",
            "board_name",
            "platform",
            "payload_path",
            "actions",
        ]
with open('suggested_plan_file.json', 'r') as f:
    data = json.load(f)
if not all(key in data for key in required_keys):
    missing_keys = [key for key in required_keys if key not in data]
    print(f"Error: Missing required keys in suggested_plan_file.json: {', '.join(missing_keys)}")
    exit(1)
print("suggested_plan_file.json is valid")
