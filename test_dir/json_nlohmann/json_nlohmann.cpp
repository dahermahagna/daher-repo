#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <fstream>

// Define a SubPointData structure
struct SubPointData {
    uint8_t id;
    std::string data;

    // Convert SubPointData to JSON
    nlohmann::json to_json() const {
        return nlohmann::json{
            {"id", id},
            {"data", data}
        };
    }

    // Create SubPointData from JSON
    static SubPointData from_json(const nlohmann::json& j) {
        return SubPointData{
            j.at("id").get<uint8_t>(),
            j.at("data").get<std::string>()
        };
    }
};

int main() {
    // Create a vector of SubPointData
    std::vector<SubPointData> subpoints = {
        {1, R"({
    "data": {
        "txid": "1746:542820:345137",
        "interface": {
            "module": [
                {
                    "v-chassis": 1,
                    "v-slot": 1,
                    "mac-address": "11:32:33:22:48:48",
                    "description": "HVS-hvs_88 OLTs",
                    "name": "HVS-hvs_88",
                    "admin-state": "up",
                    "tunnel-config": "other"
                }
            ],
            "olt-control": {
                "tunnel-mgmt": {
                    "ip-address-pool": {
                        "start-ipv4-addr": "209.201.19.20"
                    },
                    "cre-gw-ip-address": "209.201.19.1/24",
                    "dsi-vlan-id": {
                        "vid": 195
                    }
                }
            },
            "bng-data": {
                "tunnel-data": {
                    "ip-address-pool": {
                        "start-ipv4-addr": "209.199.19.20"
                    },
                    "cre-gw-ip-address": "209.199.19.1/24",
                    "dsi-vlan-id": {
                        "vid": 2019
                    }
                }
            },
            "service-provisioning": [
                {
                    "service-provisioning-id": 1,
                    "cable": {
                        "bundle": 1
                    },
                    "nsi-ip-address": "109.19.1.11",
                    "nsi-ipv6-address": "109:19:1::11",
                    "interface": {
                        "all": true
                    },
                    "vcm-reinit-policy": "device-reset"
                }
            ]
        }
    }
})"},
        {2, "Data for point 2"},
        {3, "Data for point 3"},
        {4, "Data for point 4"},
        {5, "Data for point 5"},
        {6, "Data for point 6"},
        {7, "Data for point 7"},
        {8, "Data for point 8"},
        {9, "Data for point 9"},
        {10, "Data for point 10"},
        {11, "Data for point 11"},
        {12, "Data for point 12"},
        {13, "Data for point 13"},
        {14, "Data for point 14"},
        {15, "Data for point 15"},
        {16, "Data for point 16"},
        {17, "Data for point 17"},
        {18, "Data for point 18"},
        {19, "Data for point 19"},
        {20, "Data for point 20"}
    };

    // Serialize the vector to JSON
    nlohmann::json json_subpoints = nlohmann::json();
    json_subpoints["subpoints"] = nlohmann::json::array();
    for (const auto& subpoint : subpoints) {
        json_subpoints["subpoints"].push_back(subpoint.to_json());
    }
    // nlohmann::json json_subpoints = nlohmann::json::array();
    // for (const auto& subpoint : subpoints) {
    //     json_subpoints.push_back(subpoint.to_json());
    // }


    // Simulate receiving JSON as a string
    std::string json_string/*  = json_subpoints.dump(4) */;

    // // Parse the JSON string
    // nlohmann::json parsed_json = nlohmann::json::parse(json_string);
    nlohmann::json  j = nlohmann::json::parse(subpoints[0].data)["data"];
    if(j.contains("interface"))
    {
        j["new_interface"] = j["interface"];
        j.erase("interface");

    }

    j.erase("txid");
    json_string = j.dump(4);
    // std::cout << "Serialized JSON: " << json_string << std::endl;
    std::ofstream json_file;
    json_file.open("subpoints.json",std::ios::app);
    // json_file << parsed_json.dump(4);
    json_file << json_string << "\n\n\n\n\nDAHER\n\n\n";
    
    nlohmann::json parsed_json = nlohmann::json::parse(subpoints[0].data)["data"];
    json_file << parsed_json.dump(4);    
    
    json_file.close();


    // json_file.open("subpoints.json", std::ios::app);
    // json_file << "\n\n";

    // try{
    // json_file << "{\n\"Serialized JSON\":\n" << nlohmann::json::parse(subpoints[0].data)["data"]["interface"]<<"\n}"<< std::endl;
    // }
    // catch(const nlohmann::json::parse_error& e)
    // {
    //     std::cerr << "Parse error at byte " << e.byte << ": " << e.what() << '\n';
    //     json_file << "Parse error at byte " << e.byte << ": " << e.what() << '\n';
    // }
    // json_file.close();

    // Deserialize JSON back into a vector of SubPointData
    // std::vector<SubPointData> deserialized_subpoints;
    // for (const auto& j : parsed_json) {
    //     deserialized_subpoints.push_back(SubPointData::from_json(j));
    // }

    //auto json_subpoints2 = nlohmann::json::parse(subpoints[0].data);
    // std::cout << "json_subpoints2: " << json_subpoints2.dump(4) << std::endl;
    // json_file.open("subpoints.json", std::ios::app);
    // json_file << "\n\nDAHER\n";
    // json_file.close();
    return 0;
}