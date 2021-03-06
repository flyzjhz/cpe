aa("rip_conf_rule", rip_config_rule_attr )
aa("rip_conf_rule_default", rip_config_rule_attr )
aa("fr_rule", fr_rule_attr )
aa("one_to_one_rule", one_to_one_rule_attr )
aa("vs_rule", vs_rule_attr )
aa("pt_rule", pt_rule_attr )
aa("fl_rule", fl_rule_attr )
aa("fr_dmz_rule", fr_dmz_rule_attr )
aa("fl_hwaddr_rule", fl_hwaddr_rule_attr )
aa("wan0_proto", proto_rule_attr )
aa("wan_main_rule", wan_main_rule_attr )
aa("wan_static_rule", wan_static_rule_attr )
aa("wan_dhcp_rule", wan_dhcp_rule_attr )
aa("wan_dhcpv6_rule", wan_dhcpv6_rule_attr )
aa("wan_pppoe_rule", wan_pppoe_rule_attr )
aa("wan_staticv6_rule", wan_staticv6_rule_attr )
aa("wan_tunnel6_rule", wan_tunnel6_rule_attr )
aa("wan_pppoev6_rule", wan_pppoev6_rule_attr )
aa("wan_wwan_rule", wan_wwan_rule_attr )
aa("wan_pptp_l2tp_rule", wan_pptp_l2tp_common_rule_attr )
aa("wan_pptpc_rule", wan_pptp_l2tp_rule_attr )
aa("wan_l2tpc_rule", wan_pptp_l2tp_rule_attr )
aa("wan_wwan_probe_rule", wan_wwan_probe_rule_attr )
aa("wan_bigpond_rule", wan_bigpond_rule_attr )
aa("wan_dns_rule", wan_dns_rule_attr )
aa("wan_dnsv6_rule", wan_dnsv6_rule_attr )
aa("wan_hwaddr_rule_default", wan_hwaddr_rule_default_attr)
aa("wan_hwaddr_clone_rule", wan_hwaddr_clone_rule_attr )
aa("wan_ddns_rule", wan_ddns_rule_attr )
aa("wan_opendns_rule", wan_opendns_rule_attr )
aa("wan_st_rule", wan_st_rule_attr )
aa("wan_stv6_rule", wan_stv6_rule_attr )
aa("wan_up_down_state_rule", wan_up_down_state_rule_attr )
aa("lan_st_rule", lan_st_rule_attr )
aa("lan_stv6_rule", lan_stv6_rule_attr )
aa("wlan_st_rule", wlan_st_rule_attr )
aa("lan0_proto", proto_rule_attr )
aa("lan1_proto", proto_rule_attr )
aa("lan_main_rule", lan_main_rule_attr )
aa("lan_hwaddr_rule_default", lan_hwaddr_rule_default_attr )
aa("lan_hwaddr_clone_rule", lan_hwaddr_clone_rule_attr )
aa("guest_lan_rule", guest_lan_rule_attr )
aa("guest1_lan_rule", guest_lan_rule_attr )
aa("lan_static_rule", lan_static_rule_attr )
aa("lan_staticv6_rule", lan_staticv6_rule_attr )
aa("lan_static_rule_default", lan_static_rule_attr )
aa("lan_staticv6_rule_default", lan_staticv6_rule_attr )
aa("lan_dhcps_rule", lan_dhcps_rule_attr )
aa("lan_dhcpsv6_rule", lan_dhcpsv6_rule_attr )
aa("lan_dns_rule", wan_dns_rule_attr )
/* XXX: TODO: Fix this in the future. */
aa("wl0_wds", wl0_wds_attr )
aa("bw_rule", bw_rule_attr )
aa("bw_monitor_rule", bw_monitor_rule_attr )
aa("bw_ezmodesort_rule", bw_ezmodesort_rule_attr )
aa("bw_app_rule", bw_app_rule_attr )
aa("snmpd_access_rule", snmpd_access_rule_attr )
aa("snmpd_group_rule", snmpd_group_rule_attr )
aa("snmpd_view_rule", snmpd_view_rule_attr )
aa("bw_sbm_rule", bw_sbm_rule_attr )
aa("rt_rule", rt_rule_attr )
aa("rt_rip_rule" , rt_rip_rule_attr )
aa("nat_rule" , nat_rule_attr )
aa("hwnat_rule" , nat_rule_attr )
aa("wf_content_rule", wf_content_rule_attr )
aa("wf_rule", wf_rule_attr )
aa("igmp_proxy_rule", igmp_proxy_rule_attr )
aa("igmp_snoop_rule", igmp_snoop_rule_attr )
aa("sched_rule", sched_rule_attr )
aa("sched1_rule", sched_rule_attr )
aa("fw_upgrade_rule", fw_upgrade_rule_attr )
aa("fw_info_rule", fw_info_rule_attr )
aa("cron_rule", cron_rule_attr )
aa("vlan_rule", vlan_rule_attr )
aa("vlanport_rule", vlanport_rule_attr )
aa("br_rule", br_rule_attr )

aa("br_lan_rule", br_rule_attr )
aa("br_wan_rule", br_rule_attr )
aa("lan_wan_mode_rule", lan_wan_mode_rule_attr )
aa("system_mode", system_mode_attr )

aa("ntp_rule", ntp_rule_attr )
aa("ssh_rule", ssh_rule_attr )
aa("log_rule", log_rule_attr )
aa("ttl_rule", ttl_rule_attr )
aa("tos_rule", tos_rule_attr )
aa("upnp_rule", upnp_rule_attr )
aa("upnpxml_rule", upnpxml_rule_attr )
aa("http_rule", http_rule_attr )
aa("http_rule_default", http_rule_attr )
aa("snmpd_user_rule", snmpd_user_rule_attr)
aa("snmpd_user_rule_default", snmpd_user_rule_attr)
aa("snmpd_upgrade_rule", snmpd_upgrade_rule_attr)
aa("snmpd_com2sec_rule", snmpd_com2sec_rule_attr)
aa("snmpd_com2sec_rule_default", snmpd_com2sec_rule_attr)
aa("adtool_rule", adtool_rule_attr )
aa("stats_rule", stats_rule_attr )
aa("wol_rule", wol_rule_attr )
/* add by william for ipsec start*/
/*ipsec*/
aa("ipsec_rule", ipsec_rule_attr )
aa("ipsec_status_rule", ipsec_status_rule_attr )

/*port device*/
aa("port_device_rule", port_device_rule_attr )
aa("wl_ap_device_rule", wl_ap_device_rule_attr )
aa("wl_sta_device_rule", wl_sta_device_rule_attr )

/*rogue ap*/
aa("wl_rap_rule", wl_rap_rule_attr )
aa("wl_rap_mac_rule", wl_rap_mac_rule_attr )
aa("wl_rap_oui_rule", wl_rap_oui_rule_attr )

/*pptpd*/
aa("pptpd_rule", pptpd_rule_attr )
aa("pptpd_user_rule", pptpd_user_rule_attr )

/*l2tpd*/
aa("l2tpd_rule", l2tpd_rule_attr )
aa("l2tpd_user_rule", pptpd_user_rule_attr )

/*add for bridge mode*/
aa("bridge_rule", wl_brg_mode_rule_attr)
aa("bridge_rule_default", wl_brg_mode_rule_attr)


/*add for board model*/
aa("board_model_rule", wl_board_model_rule_attr)
aa("board_model_rule_default", wl_board_model_rule_attr)

aa("smb_vpn_passthrough_rule", smb_vpn_passthrough_rule_attr )
aa("smb_ftp_passthrough_rule", smb_ftp_passthrough_rule_attr )
aa("smb_h323_passthrough_rule", smb_h323_passthrough_rule_attr )
/* add by william for ipsec end*/
aa("fw_rule", fw_rule_attr )
aa("nat_pass_rule", nat_pass_rule_attr )
aa("wl_basic_rule", wl_basic_rule_attr )
aa("wl_ap_basic_rule", wl_basic_rule_attr )
aa("wl5g_basic_rule", wl_basic_rule_attr )
aa("wl5g_ap_basic_rule", wl_basic_rule_attr )
aa("wl_origset_rule", wl_origset_rule_attr )
aa("wl1_origset_rule", wl_origset_rule_attr )
aa("wl_advanced_rule", wl_advanced_rule_attr )
aa("wl_ap_advanced_rule", wl_advanced_rule_attr )
aa("wl1_advanced_rule", wl_advanced_rule_attr )
aa("wl1_ap_advanced_rule", wl_advanced_rule_attr )
aa("wl_wds_rule", wl_wds_rule_attr )
aa("wl1_wds_rule", wl_wds_rule_attr )
aa("wl_wme_rule", wl_wme_rule_attr )
aa("wl_mode_rule", wl_mode_rule_attr )
aa("wl1_mode_rule", wl_mode_rule_attr )
aa("wl_wps_rule", wl_wps_rule_attr )
aa("wl1_wps_rule", wl_wps_rule_attr )
aa("wl_wps_sta_rule", wl_wps_sta_rule_attr )
aa("wl1_wps_sta_rule", wl_wps_sta_rule_attr )
aa("wl_led_status_rule", wl_led_status_rule_attr )

aa("wl0_basic_rule", wlv_basic_rule_attr )
aa("wl1_basic_rule", wlv_basic_rule_attr )

aa("wl0_ifname_rule", wlv_ifname_rule_attr )
aa("wl0_ssid_rule", wlv_ssid_rule_attr )
aa("wl0_sec_rule", wlv_sec_rule_attr )
aa("wl0_sec_wep_rule", wlv_sec_wep_rule_attr )
aa("wl0_sec_wpa_rule", wlv_sec_wpa_rule_attr )
aa("wl0_sec_wpa2_rule", wlv_sec_wpa2_rule_attr )

aa("wl1_ifname_rule", wlv_ifname_rule_attr )
aa("wl1_ssid_rule", wlv_ssid_rule_attr )
aa("wl1_sec_rule", wlv_sec_rule_attr )
aa("wl1_sec_wep_rule", wlv_sec_wep_rule_attr )
aa("wl1_sec_wpa_rule", wlv_sec_wpa_rule_attr )
aa("wl1_sec_wpa2_rule", wlv_sec_wpa2_rule_attr )

aa("wl00_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl00_acl_rule", wlv_acl_rule_attr )
aa("wl01_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl01_acl_rule", wlv_acl_rule_attr )
aa("wl02_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl02_acl_rule", wlv_acl_rule_attr )
aa("wl03_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl03_acl_rule", wlv_acl_rule_attr )
aa("wl04_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl04_acl_rule", wlv_acl_rule_attr )
aa("wl05_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl05_acl_rule", wlv_acl_rule_attr )
aa("wl06_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl06_acl_rule", wlv_acl_rule_attr )
aa("wl07_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl07_acl_rule", wlv_acl_rule_attr )

aa("wl10_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl10_acl_rule", wlv_acl_rule_attr )
aa("wl11_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl11_acl_rule", wlv_acl_rule_attr )
aa("wl12_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl12_acl_rule", wlv_acl_rule_attr )
aa("wl13_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl13_acl_rule", wlv_acl_rule_attr )
aa("wl14_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl14_acl_rule", wlv_acl_rule_attr )
aa("wl15_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl15_acl_rule", wlv_acl_rule_attr )
aa("wl16_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl16_acl_rule", wlv_acl_rule_attr )
aa("wl17_acl_basic_rule", wlv_acl_basic_rule_attr )
aa("wl17_acl_rule", wlv_acl_rule_attr )

aa("wl0_wds_basic_rule", wl_wds_basic_rule_attr )
aa("wl0_wds_sec_wep_rule", wl_wds_sec_wep_rule_attr )
aa("wl0_wds_sec_wpa_rule", wl_wds_sec_wpa_rule_attr )
aa("wl1_wds_basic_rule", wl_wds_basic_rule_attr )
aa("wl1_wds_sec_wep_rule", wl_wds_sec_wep_rule_attr )
aa("wl1_wds_sec_wpa_rule", wl_wds_sec_wpa_rule_attr )

//STA General
aa("wl0_apcli_rule", wl_apcli_rule_attr )
aa("wl1_apcli_rule", wl_apcli_rule_attr )
//STA Security WEP
aa("wl0_apcli_sec_wep_rule", wl_apcli_sec_wep_rule_attr )
aa("wl1_apcli_sec_wep_rule", wl_apcli_sec_wep_rule_attr )
//STA Security PSK
aa("wl0_apcli_sec_wpa_rule", wl_apcli_sec_wpa_rule_attr )
aa("wl1_apcli_sec_wpa_rule", wl_apcli_sec_wpa_rule_attr )
aa("wl0_apcli_sec_wpa2_rule", wl_apcli_sec_wpa_rule_attr )
aa("wl1_apcli_sec_wpa2_rule", wl_apcli_sec_wpa_rule_attr )
//STA Security 802.1X
aa("wl0_wpa_auth_rule", wl_apcli_wpa_rule_attr)
aa("wl0_wpa_auth_rule_default", wl_apcli_wpa_rule_attr)
aa("wl1_wpa_auth_rule", wl_apcli_wpa_rule_attr)
aa("wl1_wpa_auth_rule_default", wl_apcli_wpa_rule_attr)

aa("weather_rule", weather_rule_attr )

aa("wl_easy_mode_rule", wl_ezsy_mode_rule_attr )
aa("wl_wisp_mode_rule", wl_wisp_mode_rule_attr )

aa("direct_multicast_status", direct_multicast_status_attr )
aa("direct_multicast_maxnum", direct_multicast_maxnum_attr )
aa("tos_classification_status", tos_classification_status_attr )
aa("cos_classification_status", cos_classification_status_attr )
aa("heuristics_classification_status", heuristics_classification_status_attr )
aa("bwm_level_dl_index", bwm_level_dl_index_attr )
aa("bwm_level_up_index", bwm_level_up_index_attr )
aa("qos_rule_status", qos_rule_status_attr )
aa("qos_rule_active", qos_rule_active_attr )
aa("bwm_level", bwm_level_attr )
aa("stream_priority", stream_priority_attr    )
aa("sta_priority", sta_priority_attr )
aa("qos_rule_record", qos_rule_record_attr )
aa("igmp_snooping", igmp_snooping_attr )
aa("tos_classify", tos_classify_attr )
aa("cos_classify", cos_classify_attr )
aa("heuristics_classify", heuristics_classify_attr )
aa("tos_mark", tos_mark_attr )
aa("root_bw", root_bw_attr )
aa("wl0_ap_wmm_rule", wmm_rule_attr )
aa("wl1_ap_wmm_rule", wmm_rule_attr )
aa("wl0_sta_wmm_rule", wmm_rule_attr )
aa("wl1_sta_wmm_rule", wmm_rule_attr )

