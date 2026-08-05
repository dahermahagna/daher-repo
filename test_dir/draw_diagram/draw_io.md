```mermaid
classDiagram
    %% Interfaces
    class IDocsisConfigObserver {
        <<interface>>
        +OnDocsisCommit()
        +UpdateBaseDocsSubscriberManagement(base_docs_subscriber_management)
        +SetDocsisIpPoolSize(docsisIpPoolSize)
    }

    %% Base Handler
    class configHandlerBase {
        -log4cxx::LoggerPtr _logger
        -IPAddressCollector& _addressCollector
        -unique_ptr<redi::pstream> ccap_cfg
        -IDocsisConfigObserver* _docsisConfigObserver
        +CommitAction(config, defaultsValuesIncluded)
        +SetDocsisConfigObserver(docsisConfigObserver)
    }

    %% Config Handlers
    class ConfigHandler {
        -boost::system::error_code _lastError
        -CcapConfig _cacheCcap
        -pair<string, vector<SubPointData>> _configWithDefaults
        -pair<string, vector<SubPointData>> _configWithoutDefaults
        -string _lastProcessedTxid
        -bool _isDocsisConfigReady
        +CommitAction(config, defaultsValuesIncluded)
        +SetDocsisConfigObserver(docsisConfigObserver)
        -createConfigStructures(ccapConfig)
        -CcapOnCommit(config, configCache)
        -ConfigureK8s(...)
        -ConfigureDevellog(config)
        -CommitMgmtExtConfig(mgmtExtConfig)
        -CommitTimezoneConfig(tzConfig)
        -MakeLcceConfig(...)
        -GetLcceAssignmentJson(lcces)
        -SplitIpAndPrefixLength(s)
    }

    class PonConfigHandler {
        -pair<string, PonConfig> _ponConfigCashe
        -string _base_docs_subscriber_management
        -uint32_t _docsisIpPoolSize
        +CommitAction(config, defaultsValuesIncluded)
        +isConfigPresent(configInterfaceStr)
        +PonOnCommit(config)
        +OnDocsisCommit()
        +UpdateBaseDocsSubscriberManagement(base_docs_subscriber_management)
        +SetDocsisIpPoolSize(docsisIpPoolSize)
    }

    %% IP Address Collector
    class IPAddressCollector {
        +OnConfigurationArrived()
        -TimerEvent(ec)
        -CancelTimer()
        -Schedule()
        -PollAddresses()
        -logger_
        -io_
        -_work
        -timer_
        -thread_
    }

    %% LcceManager
    class LcceManager {
        +ProcessConfiguration(config)
    }
    class LcceData {
        +LcceIp ipv4Address
        +LcceIp ipv6Address
        +set<MacDomainId> mdIds
    }

    %% Config Structs
    class CcapConfig {
        +LcceAddressPoolConfig dsiLcce
        +DsiConfig dsi
        +MgmtExtConfig mgmtExt
        +MacDomainsConfig macDomainConfig
        +TimezoneConfig tzConfig
        +AccessConfig accessConfig
        +BannersConfig bannersConfig
        +string ccapWithoutDefaults
        +string ccapWithDefaults
        +DevellogConfig devellogConfig
        +uint32_t docsisIpPoolSize
    }
    class LcceAddressPoolConfig {
        +string start_ipv4_addr
        +string start_ipv6_addr
        +uint32_t count
    }
    class DsiConfig {
        +uint16_t cre_vlan_id
        +string cre_ip_address
        +uint32_t cre_ip_prefix_length
        +string cre_ipv6_address
        +uint32_t cre_ipv6_prefix_length
        +string gcp_ip_address
        +string gcp_ipv6_address
    }
    class MgmtExtConfig {
        +bool netconf_enabled
    }
    class MacDomainsConfig {
        +set<MacDomainConfig> mds_
        +push_back(rMd)
    }
    class MacDomainConfig {
        +uint32_t mdId_
        +bool isUp
        +string macAddr
        +string lcce
    }
    class TimezoneConfig {
        +string zone_name
    }
    class AccessConfig {
        +bool ssh
        +string hostname
    }
    class BannersConfig {
        +string login
        +string motd
    }
    class DevellogConfig {
        +bool is_enabled
        +string severity
    }
    class PonConfig {
        +string interface
        +string cable_bundle
        +string cable_subbundle
        +string rip
        +string system_proto_throttle
        +string cable_proto_throttle
        +string mcast
        +string lawful_intercept
        +string ipsec
        +uint32_t docsisIpPoolSize
    }
    class LcceConfig {
        +uint32_t lcceIpPoolSize
        +string lcceIpPoolStart
        +string lcceIpv6PoolStart
        +string creIpAddress
        +uint8_t creIpPrefixLength
        +string creIpv6Address
        +uint8_t creIpv6PrefixLength
        +uint8_t redisNumber
        +map<uint32_t, string> mdLcces
    }

    %% Relationships
    configHandlerBase <|-- ConfigHandler
    configHandlerBase <|-- PonConfigHandler
    PonConfigHandler ..|> IDocsisConfigObserver : implements
    configHandlerBase o-- IDocsisConfigObserver : observer
    configHandlerBase o-- IPAddressCollector : uses
    ConfigHandler o-- CcapConfig : manages
    ConfigHandler o-- LcceManager : uses
    ConfigHandler o-- DevellogConfig : configures
    ConfigHandler o-- MgmtExtConfig : configures
    ConfigHandler o-- TimezoneConfig : configures
    ConfigHandler o-- AccessConfig : configures
    ConfigHandler o-- BannersConfig : configures
    ConfigHandler o-- MacDomainsConfig : configures
    ConfigHandler o-- DsiConfig : configures
    ConfigHandler o-- LcceAddressPoolConfig : configures
    ConfigHandler o-- LcceConfig : creates
    PonConfigHandler o-- PonConfig : manages
    LcceManager o-- LcceData : manages
    LcceManager o-- LcceConfig : processes
    MacDomainsConfig o-- MacDomainConfig : contains
    CcapConfig o-- LcceAddressPoolConfig : contains
    CcapConfig o-- DsiConfig : contains
    CcapConfig o-- MgmtExtConfig : contains
    CcapConfig o-- MacDomainsConfig : contains
    CcapConfig o-- TimezoneConfig : contains
    CcapConfig o-- AccessConfig : contains
    CcapConfig o-- BannersConfig : contains
    CcapConfig o-- DevellogConfig : contains