/*
 * DO NOT EDIT!
 * Automatically generated from dhcpcd-embedded.conf
 * Ths allows us to simply generate DHCP structure without any C programming.
 */

/*
 * dhcpcd - DHCP client daemon
 * Copyright (c) 2006-2020 Roy Marples <roy@marples.name>
 * All rights reserved

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <unistd.h>

const char dhcpcd_embedded_conf[] =
#ifdef SMALL
"define 1 request ipaddress subnet_mask\n"
"define 121 rfc3442 classless_static_routes\n"
"define 3 request array ipaddress routers\n"
"define 6 array ipaddress domain_name_servers\n"
"define 12 dname host_name\n"
"define 15 array dname domain_name\n"
"define 26 uint16 interface_mtu\n"
"define 28 request ipaddress broadcast_address\n"
"define 33 request array ipaddress static_routes\n"
"define 50 ipaddress dhcp_requested_address\n"
"define 51 request uint32 dhcp_lease_time\n"
"define 52 byte dhcp_option_overload\n"
"define 53 byte dhcp_message_type\n"
"define 54 ipaddress dhcp_server_identifier\n"
"define 55 array byte dhcp_parameter_request_list\n"
"define 56 string dhcp_message\n"
"define 57 uint16 dhcp_max_message_size\n"
"define 58 request uint32 dhcp_renewal_time\n"
"define 59 request uint32 dhcp_rebinding_time\n"
"define 60 string vendor_class_identifier\n"
"define 61 binhex dhcp_client_identifier\n"
"define 80 norequest flag rapid_commit\n"
"define 81 embed fqdn\n"
"embed bitflags=0000NEOS flags\n"
"embed byte rcode1\n"
"embed byte rcode2\n"
"embed optional domain fqdn\n"
"define 119 array domain domain_search\n"
"define 249 rfc3442 ms_classless_static_routes\n"
"definend 1 binhex source_address\n"
"definend 2 binhex target_address\n"
"definend 3 index embed prefix_information\n"
"embed byte length\n"
"embed bitflags=LAH flags\n"
"embed uint32 vltime\n"
"embed uint32 pltime\n"
"embed uint32 reserved\n"
"embed array ip6address prefix\n"
"definend 5 embed mtu\n"
"embed uint16 reserved\n"
"embed uint32 mtu\n"
"definend 25 index embed rdnss\n"
"embed uint16 reserved\n"
"embed uint32 lifetime\n"
"embed array ip6address servers\n"
"definend 31 index embed dnssl\n"
"embed uint16 reserved\n"
"embed uint32 lifetime\n"
"embed domain search\n"
"define6 1 binhex client_id\n"
"define6 2 binhex server_id\n"
"define6 3 norequest index embed ia_na\n"
"embed binhex:4 iaid\n"
"embed uint32 t1\n"
"embed uint32 t2\n"
"encap 5 option\n"
"encap 13 option\n"
"define6 4 norequest index embed ia_ta\n"
"embed uint32 iaid\n"
"encap 5 option\n"
"encap 13 option\n"
"define6 5 norequest index embed ia_addr\n"
"embed ip6address ia_addr\n"
"embed uint32 pltime\n"
"embed uint32 vltime\n"
"encap 13 option\n"
"define6 12 ip6address unicast\n"
"define6 13 norequest embed status_code\n"
"embed uint16 status_code\n"
"embed optional string message\n"
"define6 18 binhex interface_id\n"
"define6 19 byte reconfigure_msg\n"
"define6 20 flag reconfigure_accept\n"
"define6 23 array ip6address name_servers\n"
"define6 24 array domain domain_search\n"
"define6 39 embed fqdn\n"
"embed bitflags=00000NOS flags\n"
"embed optional domain fqdn\n"
"define6 82 request uint32 sol_max_rt\n"
"define6 83 request uint32 inf_max_rt\n"
#else
"define 1 request ipaddress subnet_mask\n"
"define 121 rfc3442 classless_static_routes\n"
"define 2 uint32 time_offset\n"
"define 3 request array ipaddress routers\n"
"define 4 array ipaddress time_servers\n"
"define 5 array ipaddress ien116_name_servers\n"
"define 6 array ipaddress domain_name_servers\n"
"define 7 array ipaddress log_servers\n"
"define 8 array ipaddress cookie_servers\n"
"define 9 array ipaddress lpr_servers\n"
"define 10 array ipaddress impress_servers\n"
"define 11 array ipaddress resource_location_servers\n"
"define 12 dname host_name\n"
"define 13 uint16 boot_size\n"
"define 14 string merit_dump\n"
"define 15 array dname domain_name\n"
"define 16 ipaddress swap_server\n"
"define 17 string root_path\n"
"define 18 string extensions_path\n"
"define 19 byte ip_forwarding\n"
"define 20 byte non_local_source_routing\n"
"define 21 array ipaddress policy_filter\n"
"define 22 uint16 max_dgram_reassembly\n"
"define 23 byte default_ip_ttl\n"
"define 24 uint32 path_mtu_aging_timeout\n"
"define 25 array uint16 path_mtu_plateau_table\n"
"define 26 uint16 interface_mtu\n"
"define 27 byte all_subnets_local\n"
"define 28 request ipaddress broadcast_address\n"
"define 29 byte perform_mask_discovery\n"
"define 30 byte mask_supplier\n"
"define 31 byte router_discovery\n"
"define 32 ipaddress router_solicitation_address\n"
"define 33 request array ipaddress static_routes\n"
"define 34 byte trailer_encapsulation\n"
"define 35 uint32 arp_cache_timeout\n"
"define 36 uint16 ieee802_3_encapsulation\n"
"define 37 byte default_tcp_ttl\n"
"define 38 uint32 tcp_keepalive_interval\n"
"define 39 byte tcp_keepalive_garbage\n"
"define 40 string nis_domain\n"
"define 41 array ipaddress nis_servers\n"
"define 42 array ipaddress ntp_servers\n"
"define 43 binhex vendor_encapsulated_options\n"
"define 44 array ipaddress netbios_name_servers\n"
"define 45 ipaddress netbios_dd_server\n"
"define 46 byte netbios_node_type\n"
"define 47 string netbios_scope\n"
"define 48 array ipaddress font_servers\n"
"define 49 array ipaddress x_display_manager\n"
"define 50 ipaddress dhcp_requested_address\n"
"define 51 request uint32 dhcp_lease_time\n"
"define 52 byte dhcp_option_overload\n"
"define 53 byte dhcp_message_type\n"
"define 54 ipaddress dhcp_server_identifier\n"
"define 55 array byte dhcp_parameter_request_list\n"
"define 56 string dhcp_message\n"
"define 57 uint16 dhcp_max_message_size\n"
"define 58 request uint32 dhcp_renewal_time\n"
"define 59 request uint32 dhcp_rebinding_time\n"
"define 60 string vendor_class_identifier\n"
"define 61 binhex dhcp_client_identifier\n"
"define 64 string nisplus_domain\n"
"define 65 array ipaddress nisplus_servers\n"
"define 66 dname tftp_server_name\n"
"define 67 string bootfile_name\n"
"define 68 array ipaddress mobile_ip_home_agent\n"
"define 69 array ipaddress smtp_server\n"
"define 70 array ipaddress pop_server\n"
"define 71 array ipaddress nntp_server\n"
"define 72 array ipaddress www_server\n"
"define 73 array ipaddress finger_server\n"
"define 74 array ipaddress irc_server\n"
"define 75 array ipaddress streettalk_server\n"
"define 76 array ipaddress streettalk_directory_assistance_server\n"
"define 77 binhex user_class\n"
"define 78 embed slp_agent\n"
"embed byte mandatory\n"
"embed array ipaddress address\n"
"define 79 embed slp_service\n"
"embed byte mandatory\n"
"embed ascii scope_list\n"
"define 80 norequest flag rapid_commit\n"
"define 81 embed fqdn\n"
"embed bitflags=0000NEOS flags\n"
"embed byte rcode1\n"
"embed byte rcode2\n"
"embed optional domain fqdn\n"
"define 83 embed isns\n"
"embed byte reserved1\n"
"embed bitflags=00000SAE functions\n"
"embed byte reserved2\n"
"embed bitflags=00fFsSCE dd\n"
"embed byte reserved3\n"
"embed bitflags=0000DMHE admin\n"
"embed uint16 reserved4\n"
"embed byte reserved5\n"
"embed bitflags=0TXPAMSE server_security\n"
"embed array ipaddress servers\n"
"define 85 array ipaddress nds_servers\n"
"define 86 raw nds_tree_name\n"
"define 87 raw nds_context\n"
"define 88 array domain bcms_controller_names\n"
"define 89 array ipaddress bcms_controller_address\n"
"define 90 embed auth\n"
"embed byte protocol\n"
"embed byte algorithm\n"
"embed byte rdm\n"
"embed binhex:8 replay\n"
"embed binhex information\n"
"define 91 uint32 client_last_transaction_time\n"
"define 92 array ipaddress associated_ip\n"
"define 98 string uap_servers\n"
"define 99 encap geoconf_civic\n"
"embed byte what\n"
"embed uint16 country_code\n"
"define 100 string posix_timezone\n"
"define 101 string tzdb_timezone\n"
"define 108 uint32 ipv6_only_preferred\n"
"define 116 byte auto_configure\n"
"define 117 array uint16 name_service_search\n"
"define 118 ipaddress subnet_selection\n"
"define 119 array domain domain_search\n"
"define 120 rfc3361 sip_server\n"
"define 122 encap tsp\n"
"encap 1 ipaddress dhcp_server\n"
"encap 2 ipaddress dhcp_secondary_server\n"
"encap 3 rfc3361 provisioning_server\n"
"encap 4 embed as_req_as_rep_backoff\n"
"embed uint32 nominal\n"
"embed uint32 maximum\n"
"embed uint32 retry\n"
"encap 5 embed ap_req_ap_rep_backoff\n"
"embed uint32 nominal\n"
"embed uint32 maximum\n"
"embed uint32 retry\n"
"encap 6 domain kerberos_realm\n"
"encap 7 byte ticket_granting_server_utilization\n"
"encap 8 byte provisioning_timer\n"
"define 123 binhex geoconf\n"
"define 124 binhex vivco\n"
"define 125 embed vivso\n"
"embed uint32 enterprise_number\n"
"define 136 array ipaddress pana_agent\n"
"define 137 domain lost_server\n"
"define 138 array ipaddress capwap_ac\n"
"define 139 encap mos_ip\n"
"encap 1 array ipaddress is\n"
"encap 2 array ipaddress cs\n"
"encap 3 array ipaddress es\n"
"define 140 encap mos_domain\n"
"encap 1 domain is\n"
"encap 2 domain cs\n"
"encap 3 domain es\n"
"define 141 array domain sip_ua_cs_list\n"
"define 142 array ipaddress andsf\n"
"define 143 array ip6address andsf6\n"
"define 144 binhex geoloc\n"
"define 145 array byte forcerenew_nonce_capable\n"
"define 146 embed rdnss_selection\n"
"embed byte prf\n"
"embed ipaddress primary\n"
"embed ipaddress secondary\n"
"embed array domain domains\n"
"define 150 array ipaddress tftp_servers\n"
"define 161 string mudurl\n"
"define 208 binhex pxelinux_magic\n"
"define 209 string config_file\n"
"define 210 string path_prefix\n"
"define 211 uint32 reboot_time\n"
"define 212 embed sixrd\n"
"embed byte mask_len\n"
"embed byte prefix_len\n"
"embed ip6address prefix\n"
"embed array ipaddress brip_address\n"
"define 213 domain access_domain\n"
"define 221 encap vss\n"
"encap 0 string nvt\n"
"encap 1 binhex vpn_id\n"
"encap 255 flag global\n"
"define 249 rfc3442 ms_classless_static_routes\n"
"define 252 string wpad_url\n"
"definend 1 binhex source_address\n"
"definend 2 binhex target_address\n"
"definend 3 index embed prefix_information\n"
"embed byte length\n"
"embed bitflags=LAH flags\n"
"embed uint32 vltime\n"
"embed uint32 pltime\n"
"embed uint32 reserved\n"
"embed array ip6address prefix\n"
"definend 5 embed mtu\n"
"embed uint16 reserved\n"
"embed uint32 mtu\n"
"definend 8 embed homeagent_information\n"
"embed uint16 reserved\n"
"embed uint16 preference\n"
"embed uint16 lifetime\n"
"definend 25 index embed rdnss\n"
"embed uint16 reserved\n"
"embed uint32 lifetime\n"
"embed array ip6address servers\n"
"definend 31 index embed dnssl\n"
"embed uint16 reserved\n"
"embed uint32 lifetime\n"
"embed domain search\n"
"define6 1 binhex client_id\n"
"define6 2 binhex server_id\n"
"define6 3 norequest index embed ia_na\n"
"embed binhex:4 iaid\n"
"embed uint32 t1\n"
"embed uint32 t2\n"
"encap 5 option\n"
"encap 13 option\n"
"define6 4 norequest index embed ia_ta\n"
"embed uint32 iaid\n"
"encap 5 option\n"
"encap 13 option\n"
"define6 5 norequest index embed ia_addr\n"
"embed ip6address ia_addr\n"
"embed uint32 pltime\n"
"embed uint32 vltime\n"
"encap 13 option\n"
"define6 6 array uint16 option_request\n"
"define6 7 byte preference\n"
"define6 8 uint16 elased_time\n"
"define6 9 binhex dhcp_relay_msg\n"
"define6 11 embed auth\n"
"embed byte protocol\n"
"embed byte algorithm\n"
"embed byte rdm\n"
"embed binhex:8 replay\n"
"embed binhex information\n"
"define6 12 ip6address unicast\n"
"define6 13 norequest embed status_code\n"
"embed uint16 status_code\n"
"embed optional string message\n"
"define6 14 norequest flag rapid_commit\n"
"define6 15 binhex user_class\n"
"define6 16 binhex vivco\n"
"define6 17 embed vivso\n"
"embed uint32 enterprise_number\n"
"define6 18 binhex interface_id\n"
"define6 19 byte reconfigure_msg\n"
"define6 20 flag reconfigure_accept\n"
"define6 21 array domain sip_servers_names\n"
"define6 22 array ip6address sip_servers_addresses\n"
"define6 23 array ip6address name_servers\n"
"define6 24 array domain domain_search\n"
"define6 25 norequest index embed ia_pd\n"
"embed binhex:4 iaid\n"
"embed uint32 t1\n"
"embed uint32 t2\n"
"encap 26 option\n"
"define6 26 index embed prefix\n"
"embed uint32 pltime\n"
"embed uint32 vltime\n"
"embed byte length\n"
"embed ip6address prefix\n"
"encap 13 option\n"
"encap 67 option\n"
"define6 27 array ip6address nis_servers\n"
"define6 28 array ip6address nisp_servers\n"
"define6 29 string nis_domain_name\n"
"define6 30 string nisp_domain_name\n"
"define6 31 array ip6address sntp_servers\n"
"define6 32 uint32 info_refresh_time\n"
"define6 33 array domain bcms_server_d\n"
"define6 34 array ip6address bcms_server_a\n"
"define6 36 encap geoconf_civic\n"
"embed byte what\n"
"embed uint16 country_code\n"
"define6 37 embed remote_id\n"
"embed uint32 enterprise_number\n"
"embed binhex remote_id\n"
"define6 38 binhex subscriber_id\n"
"define6 39 embed fqdn\n"
"embed bitflags=00000NOS flags\n"
"embed optional domain fqdn\n"
"define6 40 array ip6address pana_agent\n"
"define6 41 string posix_timezone\n"
"define6 42 string tzdb_timezone\n"
"define6 43 array uint16 ero\n"
"define6 49 domain mip6_hnidf\n"
"define6 50 encap mip6_vdinf\n"
"encap 71 option\n"
"encap 72 option\n"
"encap 73 option\n"
"define6 51 domain lost_server\n"
"define6 52 array ip6address capwap_ac\n"
"define6 53 binhex relay_id\n"
"define6 54 encap mos_ip\n"
"encap 1 array ip6address is\n"
"encap 2 array ip6address cs\n"
"encap 3 array ip6address es\n"
"define6 55  encap mos_domain\n"
"encap 1 domain is\n"
"encap 2 domain cs\n"
"encap 3 domain es\n"
"define6 56 encap ntp_server\n"
"encap 1 ip6address addr\n"
"encap 2 ip6address mcast_addr\n"
"encap 3 domain fqdn\n"
"define6 57 domain access_domain\n"
"define6 58 array domain sip_ua_cs_list\n"
"define6 59 string bootfile_url\n"
"define6 60 binhex bootfile_param\n"
"define6 61 array uint16 architecture_types\n"
"define6 62 embed nii\n"
"embed byte type\n"
"embed byte major\n"
"embed byte minor\n"
"define6 63 binhex geoloc\n"
"define6 64 domain aftr_name\n"
"define6 67 embed pd_exclude\n"
"embed byte prefix_len\n"
"embed binhex subnetID\n"
"define6 69 encap mip6_idinf\n"
"encap 71 option\n"
"encap 72 option\n"
"encap 73 option\n"
"define6 70 encap mip6_udinf\n"
"encap 71 option\n"
"encap 72 option\n"
"encap 73 option\n"
"define6 71 embed mip6_hnp\n"
"embed byte prefix_len\n"
"embed ip6address prefix\n"
"define6 72 ip6address mip6_haa\n"
"define6 73 domain mip6_haf\n"
"define6 74 embed rdnss_selection\n"
"embed ip6address server\n"
"embed byte prf\n"
"embed array domain domains\n"
"define6 75 string krb_principal_name\n"
"define6 76 string krb_realm_name\n"
"define6 78 embed krb_kdc\n"
"embed uint16 priority\n"
"embed uint16 weight\n"
"embed byte transport_type\n"
"embed uint16 port\n"
"embed ip6address address\n"
"embed string realm_name\n"
"define6 80 ip6address link_address\n"
"define6 82 request uint32 sol_max_rt\n"
"define6 83 request uint32 inf_max_rt\n"
"define6 89 embed s46_rule\n"
"embed bitflags=0000000F flags\n"
"embed byte ea_len\n"
"embed byte prefix4_len\n"
"embed ipaddress ipv4_prefix\n"
"embed ip6address ipv6_prefix\n"
"define6 90 ip6address s64_br\n"
"define6 91 embed s46_dmr\n"
"embed byte prefix_len\n"
"embed binhex prefix\n"
"define6 92 embed s46_v4v6bind\n"
"embed ipaddress ipv4_address\n"
"embed byte ipv6_prefix_len\n"
"embed binhex ipv6_prefix_and_options\n"
"define6 93 embed s46_portparams\n"
"embed byte offset\n"
"embed byte psid_len\n"
"embed uint16 psid\n"
"define6 94 embed s46_cont_mape\n"
"encap 89 option\n"
"encap 90 option\n"
"define6 95 embed s46_cont_mapt\n"
"encap 89 option\n"
"encap 91 option\n"
"define6 96 embed s46_cont_lw\n"
"encap 90 option\n"
"encap 92 option\n"
"define6 112 string mudurl\n"
#endif
"\0";
