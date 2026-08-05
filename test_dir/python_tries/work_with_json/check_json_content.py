import json

def ns_filter(d, namespaces):
    """
    :type d: dict[str, T]
    :type namespaces: list[str]
    :return:
    """
    def ns_filter(d):
        for k in list(d.keys()):
            if ':' in k:
                ns = k.split(':')[0]
                if ns not in namespaces:
                    del d[k]
                    continue
            if isinstance(d[k], dict):
                ns_filter(d[k])
            elif isinstance(d[k], list):
                for l in d[k]:
                    if isinstance(l, dict):
                        ns_filter(l)
    ns_filter(d)


def main():

    # Load the JSON data from a file
    with open('output', 'r') as file:
        data = json.load(file)
    iface, cable_bundle, cable_subbundle, base_docs_subscriber_management, \
        rip, system_proto_throttle, cable_proto_throttle, mcast, lawful_intercept, ipsec, docsis_ip_pool_size = data
    
    # fields_count = 11
    # padded_data = list(data) + [None] * (fields_count - len(data))
    # (
    #     iface, cable_bundle, cable_subbundle, base_docs_subscriber_management,
    #     rip, system_proto_throttle, cable_proto_throttle, mcast,
    #     lawful_intercept, ipsec, docsis_ip_pool_size
    # ) = padded_data[:fields_count]
    
    # Check if the JSON data is a list
    if isinstance(data, list):
        print("The JSON data is a list.")
    else:
        print("The JSON data is not a list.")
        return
    # Check if the list is empty
    if not data:
        print("The list is empty.")
        return
    else:
        print("The list is not empty.")

    # if 'data' in iface:
    #     print("Processing iface...")
    #     iface = iface['data'] 
    #     ns_filter(iface, ['cosm-cli-iface-config'])

    # if 'data' in cable_bundle:
    #     cable_bundle = cable_bundle['data'] 
    #     ns_filter(cable_bundle, ['cosm-cli-config'])

    # if 'data' in cable_subbundle:
    #     ns_filter(cable_subbundle, ['cosm-cli-config'])
    #     cable_subbundle = cable_subbundle['data']

    # rip = rip['data'] if 'data' in rip else rip
    # system_proto_throttle = system_proto_throttle['data'] if 'data' in system_proto_throttle else system_proto_throttle
    # cable_proto_throttle = cable_proto_throttle['data'] if 'data' in cable_proto_throttle else cable_proto_throttle
    # mcast = mcast['data'] if 'data' in mcast else mcast
    # lawful_intercept = lawful_intercept['data'] if'data' in lawful_intercept else lawful_intercept
    # ipsec = ipsec['data'] if 'data' in ipsec else ipsec

    for json_data in [iface, cable_bundle, cable_subbundle, 
                    rip, system_proto_throttle, cable_proto_throttle, mcast,
                    lawful_intercept, ipsec]:
        if 'data' in json_data:
            json_data = json_data['data']
            if json_data is iface:
                ns_filter(json_data, ['cosm-cli-iface-config'])
            if json_data is cable_bundle or json_data is cable_subbundle:
                ns_filter(json_data, ['cosm-cli-config'])


    print("iface:", iface)
    print("cable_bundle:", cable_bundle)
    print("cable_subbundle:", cable_subbundle)
    print("rip:", rip)
    print("system_proto_throttle:", system_proto_throttle)
    print("cable_proto_throttle:", cable_proto_throttle)
    print("mcast:", mcast)
    print("lawful_intercept:", lawful_intercept)
    print("ipsec:", ipsec)

if __name__ == "__main__":
    main()  
# This script checks if the JSON data in 'output' is a list and if it is empty.
# It prints appropriate messages based on the checks.
# Make sure to have the 'output' file in the same directory as this script.
# You can run this script to validate the content of the JSON file. 