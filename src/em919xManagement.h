#include <iostream>

#include "Em919xManagementClassHelper.h"

char s_DevicePath[256];
char s_InterfaceName[256];
char s_SimPin[16];

MbimTransport s_Transport;

bool configure_link(nlohmann::json const & config)
{
	/**
	 * pSession is actualy new link object
	 */

	if (config.contains("modulePath")) {
		auto tmpStr= config.at("modulePath").dump();
		std::copy(tmpStr.begin(), tmpStr.end(), s_DevicePath);
		std::cout << "Device path: " << s_DevicePath << "\n";
	} else {
		std::cout << "ERROR: Missing device (module) path!" << "\n";
		return 0;
	}

	if (config.contains("session")) {
		for (auto& [key_master, _] : config["session"].items()) {
//			  std::cout << key_master << "\n";
//			  std::cout << "\n";
			// Session data
			ConfigFileSettings tmpConfiguration;

			for (auto [key, value] : config["session"][key_master].items()) {

				if (!key.compare("sessionId")) {
					tmpConfiguration.sessionId = value;
					std::cout << tmpConfiguration.sessionId << "\n";
				} else if (!key.compare("userName")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szUserName);
					std::cout << tmpConfiguration.szUserName << "\n";
				} else if (!key.compare("password")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szPassword);
					std::cout << tmpConfiguration.szPassword << "\n";
				} else if (!key.compare("accessString")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szAccessString);
					std::cout << tmpConfiguration.szAccessString << "\n";
				} else if (!key.compare("compression")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szCompressionType);
					std::cout << tmpConfiguration.szCompressionType << "\n";
				} else if (!key.compare("authProtocol")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szAuthProtocolType);
					std::cout << tmpConfiguration.szAuthProtocolType << "\n";
				} else if (!key.compare("ipType")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szIpType);
					std::cout << tmpConfiguration.szIpType << "\n";
				} else if (!key.compare("useVlan")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szUseVlan);
					std::cout << tmpConfiguration.szUseVlan << "\n";
				} else if (!key.compare("vlanName")) {
					auto tmpStr = value.dump();
					std::copy(tmpStr.begin(), tmpStr.end(), tmpConfiguration.szVlanName);
					std::cout << tmpConfiguration.szVlanName << "\n";
				}
			}
		}
	} else {
		std::cout << "ERROR: Session(s) missing!" << "\n";
		return 0;
	}
	return 0;
}