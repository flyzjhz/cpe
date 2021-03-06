 /******************************************************************************
* Copyright (C), 2014, LiteOn Technology Corp.
* Filename: clicbk.c
* Description: CLI Handler functions
* Created by: Mario Huang
* Revision history:
* Revision  Date               Author                    Description
* -----------------------------------------------------------------------------------------------------------------
* v0.1     2014/05/05     Mario Huang              Create this file
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "cli.h"
#include "clicbk.h"
#include "clicbklib.h"
//#include "cli_sntp.h"
#include "readline.h"
#include "ethread.h"
#include "mid_detail.h"
#include "nvram.h"
#include "nvram_rule.h"

#define LONG_BUF_LEN			8192
#define SHORT_BUF_LEN			32	

extern T_BOOL8 g_su;
extern T_INT32 is_radio_exist(T_INT32 idx);
extern T_VOID change_encryption_for_11n(T_UINT8 index);
#define PROFILE (PROFILE_INDEX(pCli->unit, pCli->vap))

extern T_BOOL8 cli_debug;
extern int set_config_sta(int radio);

void init_global_config()
{
	int i = 0;
	char buf[256] = {0};
	char *value = NULL;

	//initial value
	for( i = 0; i < GLOBAL_CONFIGS_COUNT; i++ ) {
		global_configs[i].flag = NONE;
		global_configs[i].readonly = READWRITE;
		global_configs[i].item = NULL;
		global_configs[i].item_index = CLI_INDEX_NONE;
	}

	//1. init ssid
	struct item_config *ssid_item = (struct item_config *)malloc(sizeof(struct item_config));
	if (!ssid_item) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(ssid_item->name, CLI_NAME_SSID);
	strcpy(ssid_item->params[0].param_name, CLI_NAME_SSID);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 32, EZPLIB_USE_CLI);
	strcpy(ssid_item->params[0].value, buf);
	global_configs[CLI_INDEX_SSID].item = ssid_item;
	global_configs[CLI_INDEX_SSID].item_index = CLI_INDEX_SSID;


	//2. ip address
	struct item_config *ipaddr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!ipaddr) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(ipaddr->name, CLI_NAME_IPADDR);
	strcpy(ipaddr->params[0].param_name, CLI_NAME_IPADDR);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	//if (!strcmp(buf, "static")){
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 128, EZPLIB_USE_CLI);
	strcpy(ipaddr->params[0].value, buf);
	//} else {
	//	value = nvram_safe_get("lan0_ipaddr");
	//	if (value && (strlen(value) > 0) )
	//		strcpy(ipaddr->params[0].value, value);
	//}
	global_configs[CLI_INDEX_IPADDR].item = ipaddr;
	global_configs[CLI_INDEX_IPADDR].item_index = CLI_INDEX_IPADDR;

	//3. netmask
	struct item_config *netmask = (struct item_config *)malloc(sizeof(struct item_config));
	if (!netmask) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(netmask->name, CLI_NAME_NETMASK);
	strcpy(netmask->params[0].param_name, CLI_NAME_NETMASK);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	//if (!strcmp(buf, "static")){
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan_static_rule", 0, "mask", buf, 128, EZPLIB_USE_CLI);
	strcpy(netmask->params[0].value, buf);
	//}else{
	//	value = nvram_safe_get("lan0_mask");
	//	if (value)
	//		strcpy(netmask->params[0].value, value);
	//}
	global_configs[CLI_INDEX_NETMASK].item = netmask;
	global_configs[CLI_INDEX_NETMASK].item_index = CLI_INDEX_NETMASK;

	//4. connect rssi thr
	struct item_config *conn_rssi_thr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!conn_rssi_thr) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(conn_rssi_thr->name, CLI_NAME_CONNRSSIHR);
	strcpy(conn_rssi_thr->params[0].param_name, CLI_NAME_CONNRSSIHR);
	//value = nvram_safe_get("connectrssithr");
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf, 32, EZPLIB_USE_CLI);
	strcpy(conn_rssi_thr->params[0].value, buf);
	global_configs[CLI_INDEX_CONNRSSITHR].item = conn_rssi_thr;
	global_configs[CLI_INDEX_CONNRSSITHR].item_index = CLI_INDEX_CONNRSSITHR;

    //5. disconnect rssi thr
	struct item_config *disconn_rssi_thr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!disconn_rssi_thr) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(disconn_rssi_thr->name, CLI_NAME_DISCONNRSSITHR);
	strcpy(disconn_rssi_thr->params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	//value = nvram_safe_get("disconnectrssithr");
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf, 32, EZPLIB_USE_CLI);
	strcpy(disconn_rssi_thr->params[0].value, buf);
	global_configs[CLI_INDEX_DISCONNRSSITHR].item = disconn_rssi_thr;
	global_configs[CLI_INDEX_DISCONNRSSITHR].item_index = CLI_INDEX_DISCONNRSSITHR;

	//6. deviceid
	struct item_config *deviceid = (struct item_config *)malloc(sizeof(struct item_config));
	if (!deviceid) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(deviceid->name, CLI_NAME_DEVICEID);
	strcpy(deviceid->params[0].param_name, CLI_NAME_DEVICEID);
	value = nvram_safe_get("hostname");
	if (value && strcmp(value, ""))
		strcpy(deviceid->params[0].value, value);
	global_configs[CLI_INDEX_DEVICEID].item = deviceid;
	global_configs[CLI_INDEX_DEVICEID].item_index = CLI_INDEX_DEVICEID;

	//7. account
	memset(buf, 0, sizeof(buf));
	struct item_config *acount = (struct item_config *)malloc(sizeof(struct item_config));
	if (!acount) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(acount->name, CLI_NAME_ACCOUNT);
	strcpy(acount->params[0].param_name, CLI_NAME_ACCOUNT);
	ezplib_get_attr_val("http_rule", 0, "admusername", buf, 128, EZPLIB_USE_CLI);
	if (strcmp(buf, ""))
		strcpy(acount->params[0].value, buf);
	global_configs[CLI_INDEX_ACCOUNT].item = acount;
	global_configs[CLI_INDEX_ACCOUNT].item_index = CLI_INDEX_ACCOUNT;

	//8. wireless security
	memset(buf, 0, sizeof(buf));
	struct item_config *security = (struct item_config *)malloc(sizeof(struct item_config));
	if (!security) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	//param0 auth mode
	strcpy(security->name, CLI_NAME_SECURITY);

	//param1 psk
	strcpy(security->params[1].param_name, SECURITY_KEY);

    //param2 ttls/peap
	strcpy(security->params[2].param_name, SECURITY_8021X_AUTH_TYPE);

    //param4 user
    strcpy(security->params[3].param_name, SECURITY_8021X_AUTH_USERNAME);
    
    //param5 password
    strcpy(security->params[4].param_name, SECURITY_8021X_AUTH_PASSWORD);
    
	strcpy(security->params[0].param_name, SECURITY_AUTH_MODE);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", buf, 128, EZPLIB_USE_CLI);
	strcpy(security->params[0].value, buf);

	if (!strcmp(buf, "psk")) {
        //param1 psk
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "key", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[1].value, buf);
	} else if (!strcmp(buf, "psk2")) {
	    //param2 psk
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "key", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[1].value, buf);
	} else if (!strcmp(buf, "wpa")){
	    //para2 wpa_auth
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", buf, 128, EZPLIB_USE_CLI);
        strcpy(security->params[2].value, buf);

        //para3 wpa_user
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_user", buf, 128, EZPLIB_USE_CLI);
        strcpy(security->params[3].value, buf);

        //para4 wpa_passwd
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", buf, 128, EZPLIB_USE_CLI);
        strcpy(security->params[4].value, buf);
    } else if (!strcmp(buf, "wpa2"))
	{
	    //para2 wpa_auth
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", buf, 128, EZPLIB_USE_CLI);
        strcpy(security->params[2].value, buf);

        //para3 wpa_user
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_user", buf, 128, EZPLIB_USE_CLI);
        strcpy(security->params[3].value, buf);

        //para4 wpa_passwd
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", buf, 128, EZPLIB_USE_CLI);
        strcpy(security->params[4].value, buf);
	}
	
	global_configs[CLI_INDEX_SECURITY].item = security;
	global_configs[CLI_INDEX_SECURITY].item_index = CLI_INDEX_SECURITY;

	//9. dhcp
	struct item_config *dhcp = (struct item_config *) malloc(sizeof(struct item_config));
	if (!dhcp) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(dhcp->name, CLI_NAME_DHCP);
	strcpy(dhcp->params[0].param_name, CLI_NAME_DHCP);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	strcpy(dhcp->params[0].value, buf);
	global_configs[CLI_INDEX_DHCP].item = dhcp;
	global_configs[CLI_INDEX_DHCP].item_index = CLI_INDEX_DHCP;
}

void write_to_nvram()
{
	int i = 0, j = 0;
	char buf[256] = {0};
	char sec_mode[32] = {0};

	for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
		struct item_config *item = global_configs[i].item;

		if (!item)
			continue;

		//check item is writable and modified.
		if (global_configs[i].readonly && !global_configs[i].flag)
			continue;

		switch(global_configs[i].item_index) {
			case CLI_INDEX_SSID:
				if (!strcmp(item->params[0].param_name, CLI_NAME_SSID))
					ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", item->params[0].value);
				
				break;
			case CLI_INDEX_DEVICEID:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DEVICEID))
					nvram_set("hostname", item->params[0].value);
				
				break;
			case CLI_INDEX_SECURITY:
                if(cli_debug)
                    printf("Apply security: \n");
                
				for (j = 0; j < SECURITY_PARAM_NUM; j++) {
					memset(buf, 0, sizeof(buf));
					struct param_pair *pair = NULL;
					pair = &(item->params[j]);

					if (!pair)
						continue;
                    if(cli_debug)
                        printf("pair->param_name=%s, pair->value=%s\n", pair->param_name, pair->value);
                    
					if (!strcmp(pair->param_name, SECURITY_AUTH_MODE)) {
						ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", pair->value);
						strcpy(sec_mode, pair->value);
					} 
					else if (!strcmp(pair->param_name, SECURITY_KEY)) {
						if (!strcmp(sec_mode, "psk"))
							ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "key", pair->value);
						else if (!strcmp(sec_mode, "psk2"))
							ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "key", pair->value);
					}else if (!strcmp(pair->param_name, SECURITY_8021X_AUTH_TYPE)) {
                    	ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_auth", pair->value);
					}else if (!strcmp(pair->param_name, SECURITY_8021X_AUTH_USERNAME))
					{
                        ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_user", pair->value);
                    }else if (!strcmp(pair->param_name, SECURITY_8021X_AUTH_PASSWORD))
					{
                        ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_passwd", pair->value);
					}
				}
				
				break;
			case CLI_INDEX_NETMASK:
				if (!strcmp(item->params[0].param_name, CLI_NAME_NETMASK)) {
					//printf("lan0_mask: %s\n", item->params[0].value);
					nvram_set("lan0_mask", item->params[0].value);
					ezplib_replace_attr("lan_static_rule", 0, "mask", item->params[0].value);
				}
				
				break;
			case CLI_INDEX_IPADDR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_IPADDR)) {
					//printf("lan0_ipaddr: %s\n", item->params[0].value);
					nvram_set("lan0_ipaddr", item->params[0].value);
					ezplib_replace_attr("lan_static_rule", 0, "ipaddr", item->params[0].value);
				}
				
				break;
			case CLI_INDEX_DISCONNRSSITHR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DISCONNRSSITHR))
					ezplib_replace_attr("wl0_apcli_rule", 0, "disconnrssi", item->params[0].value);
				
				break;
			case CLI_INDEX_CONNRSSITHR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_CONNRSSIHR))
					ezplib_replace_attr("wl0_apcli_rule", 0, "connrssi",  item->params[0].value);
				
				break;
			case CLI_INDEX_ACCOUNT:
				if (!strcmp(item->params[0].param_name, CLI_NAME_ACCOUNT))
					ezplib_replace_attr("http_rule", 0, "admpasswd", item->params[0].value);
				
				break;
			case CLI_INDEX_DHCP:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DHCP)) {
					if (!strcmp(item->params[0].value, "dhcp")) {
						ezplib_replace_attr("lan0_proto", 0, "curr", "dhcp");
						//nvram_set("lan0_ipaddr", "");
						//nvram_set("lan0_mask", "");
					} else {
						//memset(buf, 0, sizeof(buf));
						//ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 128, EZPLIB_USE_CLI);
						ezplib_replace_attr("lan0_proto", 0, "curr", "static");
						//if (strlen(buf) <= 0) {
						//	nvram_set("lan0_ipaddr", "192.168.1.2");
						//	nvram_set("lan0_mask", "24");
						//	ezplib_replace_attr("lan_static_rule", 0, "ipaddr", "192.168.1.2");
    					//	ezplib_replace_attr("lan_static_rule", 0, "mask", "24");
						//}
					}
				}
				
				break;
			default:
				uiPrintf("Invalid param");
		}
	}
}

void free_global_config()
{
	int i = 0;

	for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
		if(global_configs[i].item) {
			if (cli_debug)
				printf("%s is freed\n", global_configs[i].item->name);
			free(global_configs[i].item);
		}
	}
}

void debug_global_config()
{
	int i = 0, j = 0;

	if (1) {
	//if (cli_debug) {
		for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
			struct item_config *item = global_configs[i].item;
			if (item) {
				printf("item: %s\n", item->name);
				for (j = 0; j < GLOBAL_ITEM_TATAL_PARAMS; j++ ) {
					struct param_pair pair = item->params[j];
					printf("param: %s, value: %s\n", pair.param_name, pair.value);
				}
			} 
		}
	}
}

int set_value(const char *name ,struct item_config *item)
{
	int i = 0, j = 0, len1, len2, max_len;
	struct item_config *im = NULL;
	struct param_pair *pair1 = NULL, *pair2 = NULL;

	for (i = 0; i < GLOBAL_CONFIGS_COUNT ; i++) {
		im = global_configs[i].item;
		if (cli_debug)
			printf("im->name: %s, i = %d\n", im->name, i);
		
		if (!im) {
			printf("im null\n");
			return T_FALSE;
		}
		if (cli_debug)
			printf("--------------%s\n", im->name);
		
		//find item_config at the global configs
		if (!strncmp(name, im->name, strlen(im->name))) {
			if (cli_debug)
				printf("find a item: %s\n", name);
			
			break;
		}
	}

	if (im) {
		for (j = 0; j < GLOBAL_ITEM_TATAL_PARAMS; j++) {
			pair1 = &(item->params[j]);

			if (!pair1 || !strlen(pair1->param_name))
				break;
			
			for (i = 0; i < GLOBAL_ITEM_TATAL_PARAMS; i++) {
				pair2 = &(im->params[i]);
				if (!strlen(pair2->param_name))
					break;
                
				if (cli_debug)
					printf("pair1: %s:, pair2: %s\n", pair1->param_name, pair2->param_name);

                len1 = strlen(pair1->param_name);
                len2 = strlen(pair2->param_name);
                max_len = (len1>len2)?len1:len2;
                
				if (!strncmp(pair1->param_name, pair2->param_name, max_len)) {
				    if (cli_debug)
					    printf("%s: old value=%s  new value=%s\n", pair1->param_name, pair2->value, pair1->value);
                    
                    strcpy(pair2->value, pair1->value);
                    
					break;
				}
			}
		}
	}
	
	return T_TRUE;
}

int get_value(const char *name, struct item_config *item)
{
	int i = 0;
	struct item_config *im = NULL;

	for (i = 0; i < GLOBAL_CONFIGS_COUNT ; i++) {
		im = global_configs[i].item;
		if (!im) {
			if (cli_debug)
			printf("im null, i = %d\n", i);
			return T_FALSE;
		}

		//find item_config at the global configs
		if (cli_debug)
			printf("name1: %s, name2: %s\n", name, im->name);
		if (!strncmp(name, im->name, strlen(im->name))) {
			if (cli_debug)
				printf("name: %s, exsisted\n", im->name);
			memcpy(item, im, sizeof(struct item_config));
			break;
		}		
	}

	return T_TRUE;
}

 /***********************************************************************
 * Function Name : quitCmdHandler
 * Description    : process quit command
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_QUIT, quit command success
 ***********************************************************************/
int quitCmdHandler(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	/* Something important need to be done before termination */

	return CLI_PARSE_QUIT;
}
//Deleted by Mario Huang
#if 0
int antennaCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	uiPrintf("Antenna: %s\n", "");
	return CLI_PARSE_OK;
}
#endif
 /***********************************************************************
 * Function Name : ipAddrCliCmdGet
 * Description    : process ip address getting command
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipAddrCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	uiPrintf("IP Address: %s\n", "");
	return CLI_PARSE_OK;
}
//Deleted by Mario Huang
#if 0
int ipMaskCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	uiPrintf("IP Subnet Mask: %s\n", "");
	return CLI_PARSE_OK;
}
#endif
 /***********************************************************************
 * Function Name : ipAddrCliCmdSet
 * Description    : process ip address setting command
 * Input         : @pCli, cli control structure
 *                    @pNxtTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipAddrCliCmdSet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl)
{

	return CLI_PARSE_NO_VALUE;
}
//Deleted by Mario Huang
#if 0
int ipMaskCliCmdSet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl)
{

	return CLI_PARSE_NO_VALUE;
}
#endif
/*****************get the MAC Address using the ioctl function*************************/
int sysAddrGet(char *dev, char *Adrs)
{
	int i, macsock;
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));

	if ((macsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("get the mac socket file descriptor faild!\n");

	strcpy(ifr.ifr_name, dev);
	if (ioctl(macsock, SIOCGIFHWADDR, &ifr)<0)
		perror("ioctl:\n");

	for (i = 0; i< 6; i++)
	{
		Adrs[i] = ifr.ifr_hwaddr.sa_data[i];
	}

	close(macsock);
	return 0;
}

int getAPConnectStatus(int radio, int *associated)
{
	int ret = 0;
	int assotmp = 0;
	char iOpMode[32] = {0};
	
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	}	
	if (!strcmp(iOpMode, "client")) {
		ret = get_sta_assoc_status(radio, &assotmp);
		if (T_FAILURE == ret) {
			*associated = 2;
		} else {
			*associated = assotmp;
		}
	} else {
		*associated = 3;
	}

	return 0;
}

#define BSSID_EMPTY                "00:00:00:00:00:00"
int getAPConnectBSSID(int radio, char *buf)
{
	int ret = 0;
	char macBuf[18] = {0};
	char iOpMode[32] = {0};
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	}
	if (!strcmp(iOpMode, "client")) {
		ret = get_sta_assoc_bssid(radio, macBuf);
		if (T_FAILURE == ret) {
			strcpy(buf, BSSID_EMPTY);
		} else {
			strcpy(buf, macBuf);
		}
	} else {
		strcpy(buf, "--");
	}

	return 0;
}

int getWIFIRSSI(int radio, char *buf) 
{
	int ret = 0;
	char rssitmp[30] = {0};
	char iOpMode[32] = {0};
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	}
	if (!strcmp(iOpMode, "client")) {
		ret = get_sta_assoc_rssi(radio, rssitmp);
		if (T_FAILURE == ret) {
			strcpy(buf, "-- dBm");
		} else {
			strcpy(buf, rssitmp);
		}
	} else {
		strcpy(buf, "--");
	}
	
	return 0; 
}

/*****************************************************************************
 Prototype    : get_eth_status
 Description  : get ethernet port status: speed and duplex
 Input        : char *status
 Output       : char *status
 Return Value : int: 0 success; -1: fail
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/8/10
    Author       : Peter
    Modification : Created function

*****************************************************************************/
int get_eth_status(char *status)
{
    unsigned int linkStatus;
	FILE *fd;
    int link, speed, duplex;

	system("ethreg -i eth0 -p 0 0x11 | awk -F '0x' '{print $3}' > /tmp/port_ethreg");
	
	if (NULL == (fd = fopen("/tmp/port_ethreg", "r"))) {	
		fclose(fd);
		return -1;
	} else {
		fscanf(fd, "%08x", &linkStatus);

		if ((linkStatus >> 10)& 0x01) {					//get Link Status
		    /* link up*/
			switch ((linkStatus >> 14) & 0x03) {				//get speed
			case 0:
                strcpy(status, "10M");
				break;
			case 1:
                strcpy(status, "100M");
				break;
			case 2:
                strcpy(status, "1000M");
				break;
			default:
                strcpy(status, "Unknown Speed");
				break;
			}
			switch ((linkStatus >> 13)& 0x01) {			//get Duplex Mode
			case 0:
                strcat(status, " Half Duplex");
				break;
			case 1:
                strcat(status, " Full Duplex");
				break;
			default:
                strcat(status, ", unknown Duplex");
				break;
			}
			
		} else {
			strcpy(status, "Link down");
		}

		fclose(fd);
	}

	system("rm -rf /tmp/port_ethreg");

    return 0; 
}

/***********************************************************************
* Function Name : wirelessStatusGet
* Description	 : process device name getting command
* Input 		: @pCli, cli control structure
*					 @pToken, token
*					 @pNxtTbl, next token
* Output		: 
* Return value	: CLI_PARSE_OK, command success
***********************************************************************/

int wirelessStatusGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char buf[64] = {0};
	Channel_t channel_get;
	int wmode = 8;
	AP_INFO ap_info_2g;
	unsigned char MacAddrTemp[6] = {0};
	int associated = 0;
	char MacAddr[18] = {0};
	char rssi[30] = {0};
    char addrmode[32]={0};
	int ret = 0;

	//wireless mode
	ezplib_get_attr_val("system_mode", 0, "name", buf, 64, EZPLIB_USE_CLI);

    if(strstr(buf, "sta"))
    {
        ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", addrmode, 32, EZPLIB_USE_CLI);

        if(*addrmode == '1')
        {
            uiPrintf("Operation Mode: WDS Station Mode\n", buf);
        }else if(*addrmode == '0')
        {
            uiPrintf("Operation Mode: Station Mode\n", buf);
        }else
        {
            uiPrintf("Operation Mode: Station Mode\n", buf);
        }
    }else
    {
        uiPrintf("Operation Mode: %s\n", buf);
    }

	//ssid
	memset(buf, 0, sizeof(buf));
	get_sta_assoc_ssid(RADIO_2G, buf);
	if (strlen(buf) > 0) {
		uiPrintf("SSID: %s\n", buf);
	} else {
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
		uiPrintf("SSID: %s\n", buf);
	}

	//channel number
	memset(&channel_get, 0, sizeof(Channel_t));
	get_current_channel(0, &channel_get);
	uiPrintf("Channel: %d\n", channel_get.chan_number);

	// Wireless Mode
	ret = get_wirelessmode(RADIO_2G, &wmode);
	if (T_FAILURE == ret) {
		if (RADIO_2G == 0) {
			uiPrintf("Wireless Mode: 802.11a/n\n");
		}
	} else {
		if (wmode == WMODE_11AN) {
			uiPrintf("Wireless Mode: 802.11a/n\n");
		}
	}

	//authentication
	memset(&ap_info_2g, 0, sizeof(AP_INFO));

	ret = get_sta_assoc_ap_info(RADIO_2G, &ap_info_2g);
	uiPrintf("Security: ");
	switch (ap_info_2g.auth_mode)
	{
		case 1:
		case 4:
			uiPrintf("WPA-PSK\n");
			break;
		case 2:
		case 5:
			uiPrintf("WPA2-PSK\n");
			break;
		case 3:
		case 6:
			uiPrintf("WPA2-Mixed-PSK\n");
			break;
		case 7:
			uiPrintf("WEP\n");
			break;
		default:
			uiPrintf("No\n");
			break;
	}

	//wireless lan mac address
	ezplib_get_attr_val("wl_sta_device_rule", 0, "sta_device", buf, 64, EZPLIB_USE_CLI);
	sysAddrGet(buf, MacAddrTemp);
	uiPrintf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
					MacAddrTemp[0], MacAddrTemp[1], MacAddrTemp[2], 
					MacAddrTemp[3],	MacAddrTemp[4], MacAddrTemp[5]);

	//associated
	//0->Disassociated, 1->Associated, 2->Unkown, 3-> --
	getAPConnectStatus(RADIO_2G, &associated);
	switch(associated) {
		case 1:
			uiPrintf("Connect Status: Associated\n");
			break;
		default:
			uiPrintf("Connect Status: Disassociated\n");
			
	}

	//bssid
	getAPConnectBSSID(RADIO_2G, MacAddr);
	uiPrintf("BSSID: %s\n", MacAddr);

	//rssi
	getWIFIRSSI(RADIO_2G, rssi);
	if (strlen(rssi) > 0)
		uiPrintf("RSSI: %s\n", rssi);
	else
		uiPrintf("RSSI: --/--\n");


    /* add ethernet status*/
    if(get_eth_status(buf)==0)
    {
        uiPrintf("Ethernet: %s\n", buf); 
    }
    
	return CLI_PARSE_OK;
}

 /***********************************************************************
 * Function Name : systemNameGet
 * Description    : process device name getting command
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int systemNameGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char sysname[256]={0};
	if(T_FAILURE == get_sysname_from_nvram(sysname))
	{
		return T_FAILURE;
	}

	uiPrintf("Device id: %s\n", sysname);
	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : systemNameSet
 * Description    : process device name setting command
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int systemNameSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char sys_name[33]={0} ;
	char *token_get = NULL;
	//char buf[256]={0};
	int token_len = 0;
	struct item_config item;

	memset(&item, 0, sizeof(struct item_config));
	//if ( tokenCount(pCli) > 1 ) {
	//	uiPrintf(SETFALIED);
	//	return CLI_PARSE_NOMESSAGE;
	//}
	//Added by Mario Huang for print
	//uiPrintf("Enter %s,%d",__func__,__LINE__);
	token_get = tokenPop(pCli);
	if ( token_get == NULL )
		return CLI_PARSE_NO_VALUE;

	token_len = strlen(token_get);
	if(token_len > 32)
	{
		uiPrintf("Device name length can not be more than 32 charactors\n");
		return CLI_PARSE_INPUT_ERROR;
	}
	else if(token_len < 0)
	{
		uiPrintf("Device name can not be empty\n");
		return CLI_PARSE_NO_VALUE;
	}
	strcpy(sys_name, token_get);
	if(T_FALSE == check_systemname(sys_name))
	{
		uiPrintf("The only valid characters for a system name are letters numbers and a hyphen -_\n");
		return CLI_PARSE_INPUT_ERROR;
	}
	//nvram_set("hostname", sys_name);
	strcpy(item.name, CLI_NAME_DEVICEID);
	strcpy(item.params[0].param_name, CLI_NAME_DEVICEID);
	strcpy(item.params[0].value, sys_name);

	set_value(CLI_NAME_DEVICEID, &item);

	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : rebootCmdHandler
 * Description    : process device reboot command
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int rebootCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	system("reboot");
	
	return CLI_PARSE_OK;

}
 /***********************************************************************
 * Function Name : accountGet
 * Description    :  get account information
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int accountGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{	
	//char nv_admpass[TMP_LEN];
	char nv_admuser[TMP_LEN];

	ezplib_get_attr_val("http_rule", 0, "admusername", nv_admuser, TMP_LEN, EZPLIB_USE_CLI);
	//ezplib_get_attr_val("http_rule", 0, "admpasswd", nv_admpass, TMP_LEN, EZPLIB_USE_CLI);

	uiPrintf("Username: %s\n", nv_admuser);
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
 * Function Name : accountSet
 * Description    :  store account information
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int accountSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	  char *new_nv_admpass = NULL;          //new password
	  //char admpass[TMP_LEN];
	  //char admuser[TMP_LEN];
	  int total_params = 1;                 //number of total params
	  int new_password_length = 0;
	  struct item_config item;

	  if ( tokenCount(pCli) != total_params ) {
		  uiPrintf(SETFALIED);
		  return CLI_PARSE_NOMESSAGE;
	  }

	  //new password
	  new_nv_admpass = tokenPop(pCli);
	  
	  if (!new_nv_admpass)
	  	  return CLI_PARSE_NO_VALUE;

	  if (!strlen(new_nv_admpass))
	  	  return CLI_PARSE_NO_VALUE;

	  //store new password
	  new_password_length = strlen(new_nv_admpass);
	  if (new_password_length < 4 || new_password_length > 64) {
			uiPrintf("Password length from 4 to 64\n");
			return CLI_PARSE_OK;
	  }

	  memset(&item, 0, sizeof(struct item_config));
	  strcpy(item.name, CLI_NAME_ACCOUNT);
	  strcpy(item.params[0].param_name, CLI_NAME_ACCOUNT);
	  strcpy(item.params[0].value, new_nv_admpass);
	  set_value(CLI_NAME_ACCOUNT, &item);
	  //ezplib_replace_attr("http_rule", 0, "admpasswd", new_nv_admpass);
	  
	  //nvram_commit();

	  return CLI_PARSE_OK;
}
/***********************************************************************
  * Function Name : rssithrSet
  * Description    :  set disconnrssithr and connrssithr
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int rssithrSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int total_params = 2; //total params
	char *disconn_rssi_str = NULL, *conn_rssi_str = NULL; //input params rssi
	int disconn_rssi = 0, conn_rssi = 0;
	char buf[32] = {0};
	struct item_config item;

	if ( tokenCount(pCli) != total_params ) {
        //uiPrintf(SETFALIED);
        uiPrintf("Usage: set rssithr conn_value disconn_value\n"); 
        uiPrintf("       conn_value -- value is integer, range from -45 to -85\n");
        uiPrintf("       disconn_value -- value is integer, range from -55 to -95\n");
        
        if(tokenCount(pCli) > total_params)
		  return CLI_PARSE_TOO_MANY;
        else
          return CLI_PARSE_TOO_FEW;
	}

    conn_rssi_str = tokenPop(pCli);
	disconn_rssi_str = tokenPop(pCli);


	if (!disconn_rssi_str || !strlen(disconn_rssi_str) ||
		!conn_rssi_str || !strlen(conn_rssi_str))
		return CLI_PARSE_TOO_FEW;

	disconn_rssi = atoi(disconn_rssi_str);
	conn_rssi = atoi(conn_rssi_str);

	#define CONN_RSSI_MAX -45
	#define CONN_RSSI_MIN -85
	if (conn_rssi > CONN_RSSI_MAX || conn_rssi < CONN_RSSI_MIN) {
		uiPrintf("Connrssithr must be from %d to %d\n", CONN_RSSI_MAX, CONN_RSSI_MIN);
		return CLI_PARSE_ERROR;
	}
	#undef CONN_RSSI_MAX
	#undef CONN_RSSI_MIN

	#define DISCONN_RSSI_MAX -55
	#define DiSCONN_RSSI_MIN -95
	if (disconn_rssi > DISCONN_RSSI_MAX || disconn_rssi < DiSCONN_RSSI_MIN) {
		uiPrintf("Disconnrssi must be from %d to %d\n", DISCONN_RSSI_MAX, DiSCONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}

	#undef DISCONN_RSSI_MAX
	#undef DiSCONN_RSSI_MIN

	if ((conn_rssi - disconn_rssi) < 10) {
		uiPrintf("Connrssithr must be greater than disconn_value 10\n");
		return CLI_PARSE_OK;
	}

	sprintf(buf, "%d", conn_rssi);
	if (strncmp(conn_rssi_str, buf, strlen(conn_rssi_str))) {
		uiPrintf("Invalid value, value must be digit\n");
		return CLI_PARSE_OK;
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", disconn_rssi);
	if (strncmp(disconn_rssi_str, buf, strlen(disconn_rssi_str))) {
		uiPrintf("Invalid value, value must be digit\n");
		return CLI_PARSE_OK;
	}

	//connect rssi thr
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].param_name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].value, conn_rssi_str);
	set_value(CLI_NAME_CONNRSSIHR, &item);

	//disconnect rssi thr
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].value, disconn_rssi_str);
	set_value(CLI_NAME_DISCONNRSSITHR, &item);

	return CLI_PARSE_OK;

usage:
        uiPrintf("Usage: set rssithr conn_value disconn_value\n"); 
        uiPrintf("       conn_value -- value is integer, range from -45 to -85\n");
        uiPrintf("       disconn_value -- value is integer, range from -55 to -95\n");
        return CLI_PARSE_ERROR;
}
   /***********************************************************************
  * Function Name : connectRssiThresholdSet
  * Description    :  store connect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int connectRssiThresholdSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int total_params = 1; //total params
	char *rssi_str = NULL; //input params rssi
	int rssi = 0;
	char buf[32] = {0};
	int disconn_rssi = 0;
	struct item_config item;
	
	if ( tokenCount(pCli) != total_params ) {
		  //uiPrintf(SETFALIED);
		  uiPrintf("Usage: set connrssithr value\n");
		  uiPrintf("       value -- value is integer, range from -45 to -85\n");
		  return CLI_PARSE_NOMESSAGE;
	}

	rssi_str = tokenPop(pCli);

	if (!rssi_str || !strlen(rssi_str))
		return CLI_PARSE_NO_VALUE;

	rssi = atoi(rssi_str);

	#define CONN_RSSI_MAX -45
	#define CONN_RSSI_MIN -85
	if (rssi > CONN_RSSI_MAX || rssi < CONN_RSSI_MIN) {
		uiPrintf("connrssithr from %d to %d\n", CONN_RSSI_MAX, CONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}
	#undef CONN_RSSI_MAX
	#undef CONN_RSSI_MIN

	memset(&item, 0, sizeof(struct item_config));
	get_value(CLI_NAME_DISCONNRSSITHR, &item);
	disconn_rssi = atoi(item.params[0].value);

	if ((rssi - disconn_rssi) < 10) {
		uiPrintf("Connrssithr must be greater than disconnrssithr 10\n");
		return CLI_PARSE_OK;
	}
	
	sprintf(buf, "%d", rssi);

	if (strncmp(rssi_str, buf, strlen(rssi_str))) {
		uiPrintf("Invalid value, value must be digit\n");
		return CLI_PARSE_OK;
	}
		
	//nvram_set("connectrssithr", rssi_str);
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].param_name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].value, rssi_str);

	set_value(CLI_NAME_CONNRSSIHR, &item);
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : connectRssiThresholdGet
  * Description    :  get connect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int connectRssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char buf[32] = {0};
	//system("echo 'Connect Rssi Threshold:' $(iwpriv sta0 get_connrssi | awk '{print $2}' | cut -c 14-)");
	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf, 32, EZPLIB_USE_CLI);

	uiPrintf("Connect Rssi Threshold: %s\n", buf);
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : disconnectRssiThresholdGet
  * Description    :  get disconnect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int disconnectRssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	//system("echo 'Disconnect Rssi Threshold:' $(iwpriv sta0 get_disconnrssi | awk '{print $2}' | cut -c 17-)");
	char buf[32] = {0};
	
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf, 32, EZPLIB_USE_CLI);

	uiPrintf("Disconnect Rssi Threshold: %s\n", buf);

	return CLI_PARSE_OK;
}

  /***********************************************************************
  * Function Name : RssiThresholdGet
  * Description    :  get  RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int RssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	//system("echo 'Disconnect Rssi Threshold:' $(iwpriv sta0 get_disconnrssi | awk '{print $2}' | cut -c 17-)");
	char buf_1[32] = {0};
    char buf_2[32] = {0};

   	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf_1, 32, EZPLIB_USE_CLI);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf_2, 32, EZPLIB_USE_CLI);

	uiPrintf("Connect Rssi Threshold: %s\n", buf_1);
	uiPrintf("Disconnect Rssi Threshold: %s\n", buf_2);

	return CLI_PARSE_OK;
}
  
  /***********************************************************************
  * Function Name : disconnectRssiThresholdSet
  * Description    :  store disconnect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int disconnectRssiThresholdSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int total_params = 1; //total params
	char *rssi_str = NULL; //input params rssi
	int rssi = 0;
	char buf[32] = {0};
	struct item_config item;
	int conn_rssi = 0;

	if ( tokenCount(pCli) != total_params ) {
		uiPrintf("Usage: set disrssithr value\n");
		uiPrintf("value is integer, range from -200 to 200\n");
		return CLI_PARSE_NOMESSAGE;
	}

	rssi_str = tokenPop(pCli);

	if (!rssi_str || !strlen(rssi_str))
	  	return CLI_PARSE_NO_VALUE;

	rssi = atoi(rssi_str);
	#define DISCONN_RSSI_MAX -55
	#define DiSCONN_RSSI_MIN -95
	if (rssi > DISCONN_RSSI_MAX || rssi < DiSCONN_RSSI_MIN) {
		uiPrintf("disconnrssi from %d to %d\n", DISCONN_RSSI_MAX, DiSCONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}

	#undef DISCONN_RSSI_MAX
	#undef DiSCONN_RSSI_MIN

	memset(&item, 0, sizeof(struct item_config));
	get_value(CLI_NAME_CONNRSSIHR, &item);
	conn_rssi = atoi(item.params[0].value);

	if ((conn_rssi - rssi) < 10) {
		uiPrintf("Connrssithr must be greater than disconnrssithr 10\n");
		return CLI_PARSE_OK;
	}
	
	sprintf(buf, "%d", rssi);

	if (strncmp(rssi_str, buf, strlen(rssi_str))) {
	   	uiPrintf("Invalid value, value must be digit\n");
	   	return CLI_PARSE_OK;
	}

	//nvram_set("disconnectrssithr", rssi_str);
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].value, rssi_str);
	set_value(CLI_NAME_DISCONNRSSITHR, &item);

	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : rssiGet
  * Description    :  get  rssi
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int rssiGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char rssi[32] = {0};
	char TempBuf[32] = {0};
	int ret = 0;

	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);

	if (!strcmp(TempBuf, "client")) {
		//param: 0, means first radio
		ret = get_sta_assoc_rssi(0, rssi);
		if (ret != 0) {
			uiPrintf("Rssi: -/-\n");
		} else {
			uiPrintf("Rssi: %s\n", rssi);
		}
	}
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : macGet
  * Description    :  get mac
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int macGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
#if 0
    //char modeBuf[SHORT_BUF_LEN] = {0};
	//char deviceBuf[SHORT_BUF_LEN] = {0};
	char hwaddr[SHORT_BUF_LEN] = {0};

	//char ifname[16] = {0};
    char device[16] = {0};
    //char hwaddr[64];
    char *if_ptr = NULL;
    //int ret = -1;

	#if 0
	/*get wireless mac address */
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", modeBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
	if (!strcmp(modeBuf, "client")) {
		ezplib_get_attr_val("wl_ap_device_rule", 0, "sta_device", deviceBuf, 
				SHORT_BUF_LEN, EZPLIB_USE_CLI);
	} 

	if (!show_hwaddr(deviceBuf, hwaddr, SHORT_BUF_LEN)) {
		uiPrintf("Wlan Mac: %s", hwaddr);
	} else {
        uiPrintf("Wlan Not enabled");
    }
	#endif
	/* get bridge-lan's mac address */
    if_ptr = nvram_safe_get("lan0_ifname");

    //PPPoE interface doesn't have MAC address
	memset(hwaddr, 0, sizeof(hwaddr));
    if (!strncmp(if_ptr, "ppp", 3)) {
        if (snprintf(device, 16, "lan0_device") >= 16)
            return -1;
        if_ptr = nvram_safe_get(device);
    }

    if (!show_hwaddr(if_ptr, hwaddr, 64)) {
 		uiPrintf("mac: %s\n", hwaddr);
    } else {
        uiPrintf("mac: --:--:--:--:--:--\n");
	}
#else
    FILE *fp;
    unsigned char buf[64];

    /* get ethernet MAC*/
    uiPrintf("Ethernet MAC: ");
    system("boarddata get mac");

    /* get Wireless MAC*/
	if(!(fp=fopen("/dev/mtdblock6","r"))){
		uiPrintf("cant open file!\n");
		return T_FAILURE;
	}

    fseek(fp,4098, SEEK_SET);

    if(fread (buf, 6, 1, fp) != 1)
    {
        printf("Read Wireless MAC error\n");

        fclose(fp);

        return CLI_PARSE_INPUT_ERROR;
    }
    fclose(fp);

    uiPrintf("Wireless MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
        buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
#endif
	return CLI_PARSE_OK;
}

  /***********************************************************************
  * Function Name : snGet
  * Description    :  get sn
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int snGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
    /* show sn*/
    system("boarddata get sn");

    return CLI_PARSE_OK;
}

  /***********************************************************************
  * Function Name : ssidGet
  * Description    :  get ssid
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ssidGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	struct item_config item;
	#if 0
    char buf[65] = {0};
	FILE *fp = NULL;
	int len = 0;
	
	system("iwconfig apclii0 | grep ESSID | sed 's/^.*ESSID:\"//g' | sed 's/\".*$//g' > /tmp/apcli_essid");

	if (NULL == (fp = fopen("/tmp/apcli_essid", "r"))) {
		uiPrintf("CPE disconnect");
		return CLI_PARSE_ERROR;
	} else {
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		rewind(fp);
		fgets(buf, len, fp);
		fclose(fp);

		uiPrintf("CPE Ssid: %s", buf);
	}
	
	return CLI_PARSE_OK;

	#else
	//char *value;
	char buf[TMP_LEN] = {0};

	ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 32, EZPLIB_USE_CLI);
	if (!strcmp(buf, "")){
		uiPrintf("disconnect\n");
	}else{
		uiPrintf("ssid: %s\n", buf);
	}

	memset(&item, 0, sizeof(struct item_config));
	get_value("ssid", &item);

	if (cli_debug)
		uiPrintf("Ram name = %s, value = %s\n", item.params[0].param_name,item.params[0].value);

	return CLI_PARSE_OK;
	#endif
}
  /***********************************************************************
  * Function Name : ssidSet
  * Description    :  set ssid
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ssidSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *ssid;
	char buf[256]={0};
	int total_params = 0;
	struct item_config item;
	
	total_params =  tokenCount(pCli);
	ssid = tokenPop(pCli);
	
	while(ssid) {
		strcat(buf, ssid);
		total_params--;
		if (total_params)
			strcat(buf, " ");
		ssid = tokenPop(pCli);
	}
	ssid = buf;
	//printf("ssid = %s\n", ssid);
	if ( !ssid || !strlen(ssid))
		return CLI_PARSE_NO_VALUE;

	#define SSID_MIN_LEN 1
	#define SSID_MAX_MAX 32
	if ( strlen(ssid) > SSID_MAX_MAX || strlen(ssid) < SSID_MIN_LEN) {
		uiPrintf("Length of ssid from 1 to 32\n");
	}
	#undef SSID_MIN_LEN
	#undef SSID_MAX_MAX

	//ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", ssid);
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_SSID);
	strcpy(item.params[0].param_name, CLI_NAME_SSID);
	strcpy(item.params[0].value, ssid);
	set_value(CLI_NAME_SSID, &item);
	//nvram_commit();
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : securityGet
  * Description    :  get wireless security
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int securityGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	#if 0
	int ret = 0;
	char TempBuf[32] = {0};
	AP_INFO ap_info;

	memset(&ap_info, 0, sizeof(AP_INFO));
	ezplib_get_attr_val("system_mode", 0, "name", TempBuf, 32, EZPLIB_USE_CLI);
	if ((!strcmp(TempBuf, "wisp1")) || (!strcmp(TempBuf, "sta0"))) {
		//param: 0, means first radio
		ret = get_sta_assoc_ap_info(0, &ap_info);
		if (ret != 0) {
			//uiPrintf("ERROR: Get AuthMode Failure\n");
			uiPrintf("CPE is not working\n");
		} else {
			switch(ap_info.auth_mode) {
				case AUTHMODE_NONE:
					uiPrintf("Security: none/");
					break;
				case AUTHMODE_WPAPSK:
					uiPrintf("Security: wpa-psk/");
					break;
				case AUTHMODE_WPA2PSK:
					uiPrintf("Security: wpa2-psk/");
					break;
				case AUTHMODE_WPAPSKWPA2PSK:
					uiPrintf("Security: WPA2-Mixed-PSK/");
					break;
				case AUTHMODE_WEP:
					uiPrintf("Security: wep/");
					break;
				default:
					uiPrintf("Security: unkown security/");
			}

			switch (ap_info.enc_type) {
				case ENCRY_NONE:
					uiPrintf("none\n");
					break;
				case ENCRY_AES:
					uiPrintf("AES\n");
					break;
				case ENCRY_TKIP:
					uiPrintf("TKIP\n");
					break;
				case ENCRY_TKIPAES:
					uiPrintf("TKIPAES\n");
					break;
				case ENCRY_WEP:
					uiPrintf("WEP\n");
					break;
				default:
					uiPrintf("unkown\n");
			}
				
		}
	}
	#else
	char auth_mode[32] = {0};
    char wpa_auth_type[32]={0};
	char enc_type[32] = {0};

	ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", auth_mode, 32, EZPLIB_USE_CLI);
	
	if (!strcmp(auth_mode, "none")) {
		uiPrintf("Security: open\n");
	}
	else if (!strcmp(auth_mode, "disabled")) {
		uiPrintf("Security: open\n");
	}
	else if (!strcmp(auth_mode, "psk")) {
		//ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", enc_type, 32, EZPLIB_USE_CLI);

        uiPrintf("Security: wpa-psk\n");
	} 
	else if (!strcmp(auth_mode, "psk2")) {
		//ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "crypto", enc_type, 32, EZPLIB_USE_CLI);

        uiPrintf("Security: wpa2-psk\n");
	}
    else if (!strcmp(auth_mode, "wpa")) {
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth_type, 32, EZPLIB_USE_CLI);

        if(*wpa_auth_type == '0')
        {
            uiPrintf("Security: wpa ttls\n");
        }else
        {
            uiPrintf("Security: wpa peap\n");
        }
	}
    else if (!strcmp(auth_mode, "wpa2")) {
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth_type, 32, EZPLIB_USE_CLI);

        if(*wpa_auth_type == '0')
        {
            uiPrintf("Security: wpa2 ttls\n");
        }else
        {
            uiPrintf("Security: wpa2 peap\n");
        }
	}
    else if (!strcmp(auth_mode, "wep")) {
		uiPrintf("Security: wep\n");
	}
	else {
        
		uiPrintf("Invalid security type!\n");
	}
	
	#endif

	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : securitySet
  * Description    :  set wireless security
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int securitySet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
#if 0 
   char *mode = NULL;     //auth mode value
   char *enc_type = NULL; //ecryption type value
   char *psk = NULL;      //psk value
   char *option_m = NULL; //auth mode
   char *option_e = NULL; // encryption type
   char *option_p = NULL; //pre-shared key
   int total_param = 6;   //total params
   int mode_int = 0;
   int enc_type_int = 0;
   struct item_config item;
   
   if ( tokenCount(pCli) != total_param) {
	   uiPrintf("Usage: set security -m auth -e enc_type -p psk\n");
	   uiPrintf("-m auth mode. 0-none, 1-WPAPSK, 2-WPA2PSK, 3-WPA/WPA2PSK\n");
	   uiPrintf("-e encryption type. 1-TKIP, 2-AES, 3-TKIPAES\n");
	   uiPrintf("-p pre-shared key\n");
	   return CLI_PARSE_NOMESSAGE;
   }

   //option: m
   option_m = tokenPop(pCli);
   //auth mode
   mode = tokenPop(pCli);
   //option: e
   option_e = tokenPop(pCli);
   //encryption type
   enc_type = tokenPop(pCli);
   //option: p
   option_p = tokenPop(pCli);
   //pre-shared key
   psk = tokenPop(pCli);
   
   if ( !mode || !enc_type || !psk || !option_m || !option_e || !option_p )
	   return CLI_PARSE_NO_VALUE;

   #define OPTION_M "-m"
   #define OPTION_E "-e"
   #define OPTION_P "-p"

   if (strncmp(option_m, OPTION_M, strlen(OPTION_M)) ||
   	    strncmp(option_e, OPTION_E, strlen(OPTION_E)) ||
   	     strncmp(option_p, OPTION_P, strlen(OPTION_P))) {
	   return CLI_PARSE_INVALID_PARAMETER;
   }
   #undef OPTION_M
   #undef OPTION_E
   #undef OPTION_P

   if (!strlen(mode) || !strlen(enc_type))
   	   return CLI_PARSE_NO_VALUE;

   if (strlen(psk) < 8) {
   	   uiPrintf("Pre shared key should be larger than 8\n");
	   return CLI_PARSE_OK;
   }

   memset(&item, 0, sizeof(struct item_config));
   strcpy(item.name, CLI_NAME_SECURITY);
   //first parameter
   strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
   //second parameter
   strcpy(item.params[1].param_name, SECURITY_ENC_TYPE);
   //third parameter
   strcpy(item.params[2].param_name, SECURITY_KEY);

   mode_int = atoi(mode);
   enc_type_int = atoi(enc_type);
   if (cli_debug)
   	  printf("authmode = %d, encrptiontype = %d, psk = %s\n",
   	  		mode_int, enc_type_int, psk);
   
   if (mode_int == AUTHMODE_WEP) {
	   strcpy(item.params[0].value, "disabled");
   } 
   else if (mode_int == AUTHMODE_WPAPSK) {
	   strcpy(item.params[0].value, "psk");
	   if (enc_type_int == ENCRY_TKIP) {
		   strcpy(item.params[1].value, "tkip");
	   } 
	   else if (enc_type_int == ENCRY_AES) {
		   strcpy(item.params[1].value, "aes");
	   } 
	   strcpy(item.params[2].value, psk);
    } 
	else if ((mode_int == AUTHMODE_WPA2PSK) || (mode_int == AUTHMODE_WPAPSKWPA2PSK)) {
	   strcpy(item.params[0].value, "psk2");
	   if (enc_type_int == ENCRY_TKIP) {
		  strcpy(item.params[1].value, "tkip");
	   } 
	   else if (enc_type_int == ENCRY_AES) {
	      strcpy(item.params[1].value, "aes");
	   } 
	    strcpy(item.params[2].value, psk);
	} else if (mode_int == AUTHMODE_NONE){
		strcpy(item.params[0].value, "none");
		strcpy(item.params[1].value, "");
		strcpy(item.params[2].value, "");
	} else {
		uiPrintf("Invalid security, authMode: %s, encType: %s\n", 
			mode, enc_type);
	}

	set_value(CLI_NAME_SECURITY, &item);

	return CLI_PARSE_OK;
#else
    int argc = tokenCount(pCli);
    int len_psk;
    char *option_m=NULL;
    char *option_2=NULL;
    char *option_3=NULL;
    char *option_4=NULL;
    struct item_config item;
    
    memset(&item, 0, sizeof(struct item_config));


	//uiPrintf("argc=%d\n", argc);
    
    if(argc < 1){
	    goto security_usage;
    }

   if(option_m = tokenPop(pCli)){
        if (strcmp(option_m, "open")==0){
            if(argc != 1)
            {
                goto security_usage;
            }
            
            strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
            strcpy(item.params[0].value,  "disabled");
        }else if(strcmp(option_m, "wpa-psk")==0 || strcmp(option_m, "wpa2-psk")==0){
            if(argc != 2)
            {
                goto security_usage;
            }
           //pre-shared key
            option_2 = tokenPop(pCli);
           
            len_psk=strlen(option_2);
            if(len_psk < 8 || len_psk > 64){
        		uiPrintf("The length of pre-shared key should be from 8 to 64\n");

                return CLI_PARSE_NOMESSAGE;
            }

            /* should be hex digital*/
            if(len_psk == 64 && is_hex_digital_str(option_2)!= T_TRUE){
        		uiPrintf("The pre-shared key should be hex when length is 64 bytes\n");

                return CLI_PARSE_NOMESSAGE;
            }

            strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
            if(strcmp(option_m, "wpa-psk")==0){
                strcpy(item.params[0].value,"psk"); /* wpa-psk*/
            }else{
                strcpy(item.params[0].value,"psk2"); /* wpa2-psk*/
            }
            
            strcpy(item.params[1].param_name, SECURITY_KEY);
            strcpy(item.params[1].value, option_2); /* key*/
            
        }else if(strcmp(option_m, "wpa")==0 || strcmp(option_m, "wpa2")==0){
            if(argc != 4)
            {
                goto security_usage;
            }

            option_2 = tokenPop(pCli);

            if(!(strcmp(option_2, "peap")==0 || strcmp(option_2, "ttls")==0))
            {
                goto security_usage;
            }

            option_3 = tokenPop(pCli);

            if((strlen(option_3) > 64))
            {
                uiPrintf("The length user name should no more than 64\n");

                return CLI_PARSE_NOMESSAGE;
            }

            option_4 = tokenPop(pCli);

            if((strlen(option_4) > 64))
            {
                uiPrintf("The length password should no more than 64\n");

                return CLI_PARSE_NOMESSAGE;
            }

            strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
            strcpy(item.params[0].value, option_m); /* wpa/wpa2*/

            strcpy(item.params[1].param_name, SECURITY_8021X_AUTH_TYPE);
            if(strcmp(option_2, "ttls")==0)
            {
                strcpy(item.params[1].value, "0"); /* ttls*/
            }else if(strcmp(option_2, "peap")==0)
            {
                strcpy(item.params[1].value, "1"); /* peap*/
            }

            strcpy(item.params[2].param_name, SECURITY_8021X_AUTH_USERNAME);
            strcpy(item.params[2].value, option_3); /* user*/

            strcpy(item.params[3].param_name, SECURITY_8021X_AUTH_PASSWORD);
            strcpy(item.params[3].value, option_4); /* password*/
            
        }
        else if (strcmp(option_m, "wep")==0){
            uiPrintf("wep isn't supported at command line\n");
            goto security_usage;
        }else{
            goto security_usage;
        }
   }

	set_value(CLI_NAME_SECURITY, &item);

	return CLI_PARSE_OK;

security_usage:
    uiPrintf("Usage: set security open\n"
             "       set security wpa-psk/wpa2-psk passphrase\n"
             "       set security wpa/wpa2 ttls/peap username password\n");
    
    return CLI_PARSE_NOMESSAGE;

#endif

}
   /***********************************************************************
  * Function Name : ipmaskGet
  * Description    :  get netmask
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ipmaskGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *value;
	char value2[32] = {0};
	char buf[32];
	struct item_config item;

	//ezplib_get_attr_val("lan0_proto", 0, "curr", value2, 32, EZPLIB_USE_CLI);
	memset(&item, 0, sizeof(struct item_config));
	get_value(CLI_NAME_DHCP, &item);
	if (!strcmp(item.params[0].value, "static")){
		ezplib_get_attr_val("lan_static_rule", 0, "mask", buf, 32, EZPLIB_USE_CLI);
		if (buf == NULL)
			uiPrintf("NetMask: --\n");
		else{
			if(!strcmp(buf, ""))
				uiPrintf("NetMask: --\n");
			else
				uiPrintf("NetMask: %s\n", buf);
		}
	}else{
		value = nvram_safe_get("lan0_mask");
		if (value ==NULL)
			uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
		else
		{
			if(!strcmp(value, ""))
				uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
			else
				uiPrintf("NetMask: %s\n", value);
		}
	}
	
	return CLI_PARSE_OK;
}
/***********************************************************************
  * Function Name : ipmaskSet
  * Description    :  set netmask
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ipmaskSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *net_mask;
	char value[32] = {0};
	char *br_get=NULL;
	char br_name[32]={0};
	struct item_config item;

	memset(&item, 0, sizeof(struct item_config));
	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;
	}

	net_mask = tokenPop(pCli);
	if ( !net_mask || !strlen(net_mask) )
		return CLI_PARSE_NO_VALUE;

	if ( is_integer(net_mask) && atoi(net_mask) > 0 && atoi(net_mask) <= 32) {
		//ezplib_get_attr_val("lan0_proto", 0, "curr", value, 32, EZPLIB_USE_CLI);
		memset(&item, 0, sizeof(struct item_config));
		get_value(CLI_NAME_DHCP, &item);

		if (!strcmp(item.params[0].value, "static")) {
			strcpy(item.name, CLI_NAME_NETMASK);
			strcpy(item.params[0].param_name, CLI_NAME_NETMASK);
			strcpy(item.params[0].value, net_mask);
			set_value(CLI_NAME_NETMASK, &item);
		}
		else{
			uiPrintf("Can not modify Net Mask when DHCP client enabled\n");
			
			return CLI_PARSE_ERROR;
		}
		
		return CLI_PARSE_OK;
	} else {
		uiPrintf("Invalid Net Mask\n");
		
		return CLI_PARSE_NOMESSAGE;
	}
	
	return CLI_PARSE_OK;
}
/***********************************************************************
* Function Name : dhcpGet
* Description	 :  get dhcp status
* Input 		: @pCli, cli control structure
*					 @pToken, token
*					 @pNxtTbl, next token
* Output		: 
* Return value	: CLI_PARSE_OK, command success
***********************************************************************/
int dhcpGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char buf_proto[32] = {0};
	
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf_proto, 32, EZPLIB_USE_CLI);

	if (!strcmp(buf_proto, "dhcp"))
		uiPrintf("DHCP: enable\n");
	else 
		uiPrintf("DHCP: disable\n");
	
	return CLI_PARSE_OK;
}
/***********************************************************************
 * Function Name : dhcpSet
 * Description    : enable or disable dhcp function
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int dhcpSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *dhcp = NULL;
	struct item_config item;

	memset(&item, 0, sizeof(struct item_config));
	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;
	}

	dhcp = tokenPop(pCli);
	if ( !dhcp || !strlen(dhcp) )
		return CLI_PARSE_NO_VALUE;

	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, "dhcp");
	strcpy(item.params[0].param_name, "dhcp");

	if (!strcmp(dhcp, "enable")) {
		strcpy(item.params[0].value, "dhcp");
		set_value("dhcp", &item);
	} else if (!strcmp(dhcp, "disable")){
		strcpy(item.params[0].value, "static");
		set_value("dhcp",&item);
	} else {
		uiPrintf("Invalid value\n");
	}

	return CLI_PARSE_OK;
}


 /***********************************************************************
 * Function Name : ipaddrGet
 * Description    : get ip address from nvram
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipaddrGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *value;
	char value2[32] = {0};
	char buf[32];
	struct item_config item;
	
	memset(&item, 0, sizeof(struct item_config));
	get_value(CLI_NAME_DHCP, &item);
	//ezplib_get_attr_val("lan0_proto", 0, "curr", value2, 32, EZPLIB_USE_CLI);
	if (!strcmp(item.params[0].value, "static")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 32, EZPLIB_USE_CLI);
		if (buf == NULL)
			uiPrintf("IP Address: 0.0.0.0\n");
		else{
			if(!strcmp(buf, ""))
				uiPrintf("IP Address: 0.0.0.0\n");
			else
				uiPrintf("IP Address: %s\n", buf);
		}
		memset(&item, 0, sizeof(struct item_config));
		get_value(CLI_NAME_IPADDR, &item);
		if (cli_debug)
			uiPrintf("name: %s, param name: %s, param value: %s\n", item.name, item.params[0].param_name, item.params[0].value);
	}else{
		value = nvram_safe_get("lan0_ipaddr");
		if (value ==NULL)
			uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
		else
		{
			if(!strcmp(value, ""))
				uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
			else
				uiPrintf("IP Address: %s\n", value);
		}
	}
	return CLI_PARSE_OK;
}
 
 /***********************************************************************
 * Function Name : ipAddrSet
 * Description    : set ip address
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipAddrSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *pIP;
	char value[32] = {0};
	char *br_get=NULL;
	char br_name[32]={0};
	struct item_config item;

	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;

	}

	pIP = tokenPop(pCli);
	if ( pIP == NULL )
		return CLI_PARSE_NO_VALUE;

	if ( verifyIP(pIP) == 0 ) {
		uiPrintf("Invalid IP Address\n");
		return CLI_PARSE_NOMESSAGE;
	}

	if ( IsValidIpAddress(pIP) ) {
		//ezplib_get_attr_val("lan0_proto", 0, "curr", value, 32, EZPLIB_USE_CLI);
		memset(&item, 0, sizeof(struct item_config));
		get_value(CLI_NAME_DHCP, &item);

		if (!strcmp(item.params[0].value, "static")) {
			memset(&item, 0, sizeof(struct item_config));
			strcpy(item.name, CLI_NAME_IPADDR);
			strcpy(item.params[0].param_name, CLI_NAME_IPADDR);
			strcpy(item.params[0].value, pIP);
			set_value(CLI_NAME_IPADDR, &item);
		}
		else{
			uiPrintf("Can not modify IP address when DHCP client enabled\n");
			
			return CLI_PARSE_ERROR;
		}
		
		return CLI_PARSE_OK;
	} else {
		uiPrintf("Invalid IP Address\n");
		
		return CLI_PARSE_NOMESSAGE;
	}
}
 /***********************************************************************
 * Function Name : cancelcfgCmdHandler
 * Description    : restore data
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int cancelcfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	init_global_config();

	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : cancelcfgCmdHandler
 * Description    : restore data
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/ 
int accessCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	return CLI_PARSE_ACCESS_LINUX;
}
 /*
 * Private Function
 * parse the mac  and store into *addr.
 * the format is HH:HH:HH:HH:HH:HH
 */
 #define MAC_ADDR_LEN 17
int inet_atonmac(const char *s, char *addr, int addr_len)
{

    char Tmac[] = "HH:HH:HH:HH:HH:HH";
    unsigned char mac_addr[18];
    char *tmp = mac_addr;
    char c;
    char *ptr = (char *)s;
    int val = 0;
    int i, j;
    int allzero = 1;
    int all0xff = 1;

    memset(mac_addr,0,sizeof(mac_addr));

    if(addr_len != 17)
        return -1;

    i = j = 0;
    while(1){
        c = *ptr++;
        if ( Tmac[i] == 'H' ){
            
            /* must be hex char. */
            val = val * 16;
            if ( isdigit(c) ){
                val += c - '0';
                mac_addr[i] = c;
            }else if(c >= 'a' && c <= 'f'){
                val += c - 'a' + 10;
                mac_addr[i] = c;
            }else if(c >= 'A' && c <= 'F'){
                val += c - 'a' + 10;
                mac_addr[i] = c;
            }else
                return -1;
        } else{
            /* here should be : */
            if(j == 0){
                int x;
                
                //donn't distinguish globle or local mac
                x = val & 0x01;
                if(x != 0)
                    return -1;
            }
            
            if(val != 0){
                allzero = 0;
            }

            val = 0;
            j++;
            if(j == 6){
                if ((c != '\0') || allzero){
                    return -1;
                }

                memcpy(addr,mac_addr,addr_len);
                return 0;
            }
            if(c != ':'){
                return 0;
            }else{
                mac_addr[i] = c;
            }
        }
        ++i;
    }   

	return 0;
}


 /***********************************************************************
 * Function Name : mfgCmdHandler
 * Description    : mfg
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int mfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *param = NULL, *subcmd = NULL;
	unsigned char m_addr[18] = {0};
    char *tmp_addr = m_addr;
	unsigned char buf[256] = {0};

	if ( tokenCount(pCli) > 2 || tokenCount(pCli) == 0) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;
	}

	subcmd = tokenPop(pCli);

    if(subcmd)
	    param = tokenPop(pCli);

	if (!strcmp(subcmd, "setmac")) {
        FILE *fp;
        unsigned char mac_addr[6], cmd[100];
        int i;

    	if (!param)
    		return CLI_PARSE_INVALID_PARAMETER;
        
		if (strlen(param) != MAC_ADDR_LEN) {
			uiPrintf("Invalid MAC address\n");
			return CLI_PARSE_INPUT_ERROR;
		}

        /* check if valid mac*/
		if (inet_atonmac(param, tmp_addr, MAC_ADDR_LEN) < 0) {
			uiPrintf("Invalid MAC address\n");
			return CLI_PARSE_INPUT_ERROR;
		}

        /* if orignal mac is invalid, format board data*/
        system("boarddata get mac > /tmp/bd_getmac.log");

    	if(!(fp=fopen("/tmp/bd_getmac.log","r"))){
    		uiPrintf("cant open file!\n");
    		return T_FAILURE;
    	}

        fgets(buf, 32, fp);

        fclose(fp);

        if(strlen(buf)!= (MAC_ADDR_LEN+1))
        {
            uiPrintf("Orignal mac is invalid, boarddata init...\n");
            system("boarddata init");
        }

		/* save to boarddata*/
		sprintf(buf, "boarddata set mac %s", tmp_addr);
		system(buf);

        /* 
         * set wireless mac
         *     1st wifi mac = base mac + 1;
         */
        /* convert to ascii*/
        for ( i = 0; i < 6; i++ )
        {
            param[i*3+2] = '\0';
            mac_addr[i] = strtol(&param[i*3], NULL, 16);
        }

        /* base mac + 1 */
        mac_addr[5] += 1;

        /* write to 1st calibration data area*/
        sprintf((char *)cmd, "echo -n -e \"\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x\" | dd bs=1 of=/dev/mtdblock6 %s >/dev/null 2>&1",
                mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
                "seek=4098 conv=notrunc");
        
        system((char *)cmd);


        /* get ethernet MAC*/
	    uiPrintf("Ethernet MAC: ");
        system("boarddata get mac");

        /* get Wireless MAC*/
    	if(!(fp=fopen("/dev/mtdblock6","r"))){
    		uiPrintf("cant open file!\n");
    		return T_FAILURE;
    	}

        memset(buf, 0, 7);

        fseek(fp,4098, SEEK_SET);

        if(fread (buf, 6, 1, fp) != 1)
        {
            printf("Read Wireless MAC error\n");

            fclose(fp);

            return CLI_PARSE_INPUT_ERROR;
        }
        fclose(fp);

        uiPrintf("Wireless MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
            buf[0], buf[1], buf[2],buf[3], buf[4], buf[5]);

        
	}
    else  if(!strcmp(subcmd, "getmac"))
	{
	    FILE *fp;

        /* get ethernet MAC*/
	    uiPrintf("Ethernet MAC: ");
        system("boarddata get mac");

        /* get Wireless MAC*/
    	if(!(fp=fopen("/dev/mtdblock6","r"))){
    		uiPrintf("cant open file!\n");
    		return T_FAILURE;
    	}

        memset(buf, 0, 7);

        fseek(fp,4098, SEEK_SET);

        if(fread (buf, 6, 1, fp) != 1)
        {
            printf("Read Wireless MAC error\n");

            fclose(fp);

            return CLI_PARSE_INPUT_ERROR;
        }
        fclose(fp);

        uiPrintf("Wireless MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
            buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

    }
    else  if(!strcmp(subcmd, "getsn")){
    
        /* show sn*/
        system("boarddata get sn");
	}else  if(!strcmp(subcmd, "setsn")){
    	if (!param)
    		return CLI_PARSE_INVALID_PARAMETER;

        if(strlen(param) > 32)
        {
            printf("Error: SN length should be less than 33!\n");
            return CLI_PARSE_INVALID_PARAMETER;
        }

        sprintf(buf, "boarddata set sn %s", param);
    
        /* set sn*/
        system(buf);
	}
    else  if(!strcmp(subcmd, "art"))
	{
        /* mfg art IP*/

    	if (!param || !subcmd)
    		return CLI_PARSE_INVALID_PARAMETER;
        
        if(IsValidIpAddress(param))
        {
            /* 
             * call script to download art and start it
             *     art2 IP nart.out art-wasp.ko
             */
            sprintf(buf, "art2 %s nart.out art-wasp.ko", param);

            system(buf);

        }else
        {
            printf("Please specify correct TFTP server IP address!\n"
                "Usage:\n"
                "    mfg art tftp_server_ip\n");
        }
    }
    else  if(!strcmp(subcmd, "eth"))
    {
        char status[100]={0};

        if(get_eth_status(status)==0)
        {
            uiPrintf("%s\n", status); 
        }
    }
    else  if(!strcmp(subcmd, "buttontest") || !strcmp(subcmd, "button"))
    {
        struct stat bt_flag_stat;
        int wait_secs=20;


        uiPrintf("Please press button in 20 seconds!\n");

        system("[ -f /tmp/button_detected ] && rm /tmp/button_detected -fr");
        system("echo test>  /tmp/button_test");

        while(wait_secs--)
        {
            if(stat("/tmp/button_detected",&bt_flag_stat)==0)
            {
                /* button pressed detect*/
                uiPrintf("Press button detected!\n");
                return CLI_PARSE_OK;
            }else
            {
                /* button isn't pressed*/
            }

            usleep(500000);
        }

        uiPrintf("Press button isn't detected in 20 seconds!\n");

        system("[ -f /tmp/button_test ] && rm /tmp/button_test -fr");
        system("[ -f /tmp/button_detected ] && rm /tmp/button_detected -fr");
    }
    else
    {

        return CLI_PARSE_INPUT_ERROR;
    }

	return CLI_PARSE_OK;
}

 /***********************************************************************
 * Function Name : debugCmdHandler
 * Description    : print debug information
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int debugCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *debug = NULL;
	
	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;

	}

	debug = tokenPop(pCli);
	if ( debug == NULL )
		return CLI_PARSE_NO_VALUE;

	if (atoi(debug) == 1) {
		cli_debug = 1;
	} else {
		cli_debug = 0;
	}

	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : applycfgCmdHandler
 * Description    : store data to flash
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
 int applycfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
 {
    char cmd[128] = {0};
	char *sys_name = NULL;
	char *connrssi = NULL;
	char *disconnrssi = NULL;
	char buf[32] = {0};
	char br_name[32]={0}, *br_get = NULL;
	char buf_proto[32] = {0};

	//write to nvram
	write_to_nvram();
	
    //save to flash forever
 	nvram_commit();

	//restart network
	if (set_config_sta(RADIO_2G)) {
		printf("set_config_sta error!\n");
	}

	//set system name
	sys_name = nvram_safe_get("hostname");
	sprintf(cmd, "echo %s > /proc/sys/kernel/hostname",sys_name);
	system(cmd);

	//connectrssi and disconnectrssi
	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf, 32, EZPLIB_USE_CLI);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "iwpriv sta0 connrssi %d", (atoi(buf) + 95));
	//printf("%s\n", cmd);
	system(cmd);

	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf, 32, EZPLIB_USE_CLI);
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "iwpriv sta0 disconnrssi %d", (atoi(buf) + 95));
	//printf("%s\n", cmd);
	system(cmd);

	//set ipaddr
	system("/etc/init.d/network-lan restart 2>/dev/null 1>/dev/null &");
	
	return CLI_PARSE_OK;
 }

#define SINGLE_RADIO
#define RADIO_2G        0
#define RADIO_5G        1
#define WLAN_MODE_AP    0
#define WLAN_MODE_STA   1
#define T_SUCCESS  0
#define T_FAILURE  -1

int convert_vap_id(int radio, unsigned char vap_id)
{
	 int actual_id = 0;
	 
#ifdef SINGLE_RADIO
	 actual_id = vap_id*2;
#else
 
	 if(RADIO_2G == radio)
		 {
			 actual_id = vap_id*2;
		 }
	 else if(RADIO_5G == radio)
		 {
			 actual_id = vap_id*2 + 1;
		 }
#endif
	 return actual_id;
}

 /**
  * \brief	Construct VAP Interface Name according to radio(2.4G/5G), 
  * 		vapi_id(0~7) and mode(WLAN_MODE_AP|WLAN_MODE_STA)
  * \return T_SUCCESS on Success, T_FAILURE on Failure
  * \param[out] name the generated interface name
  * \param[in] radio RADIO_2G or RADIO_5G
  * \param[in] vap_id 0~7
  * \param[in] mode WLAN_MODE_AP or WLAN_MODE_STA
  */
 int construct_vap(char* name, unsigned int radio, unsigned char vap_id, unsigned char mode)
 {
	 int actual_id;
 
	 actual_id = convert_vap_id(radio, vap_id); 
	 if(RADIO_2G == radio)
		 {
			 if(WLAN_MODE_AP == mode)
				 {
				   sprintf(name, "%s%d", AP_NAME_2G, actual_id);
				 }
			 else if(WLAN_MODE_STA == mode)
				 {
				   sprintf(name, "%s%d", STA_NAME_2G, actual_id);
				 }
			 else
				 {
				   printf("ERROR:Construct vap error!\n");
					return T_FAILURE;
				 }
		 }
	 else if(RADIO_5G == radio)
	   {
			 if(WLAN_MODE_AP == mode)
				 {
				   sprintf(name, "%s%d", AP_NAME_5G, actual_id);
				 }
			 else if(WLAN_MODE_STA == mode)
				 {
				   sprintf(name, "%s%d", STA_NAME_5G, actual_id);
				 }
			 else
				 {
				   printf("ERROR:Construct vap error!\n");
					return T_FAILURE;
				 }
	   }
	 else
		 {
			 printf("ERROR:Construct vap error!\n");
			 return T_FAILURE;
		 }
 
	 return T_SUCCESS;
 }

 /***********************************************************************
* Function Name : scanDeviceApHandler
* Description	 : restore config to factory
* Input 		: @pCli, cli control structure
*					 @pToken, token
*					 @pNxtTbl, next token
* Output		: 
* Return value	: CLI_PARSE_OK, command success
***********************************************************************/
#define SCAN_AP_DONE_FILE "/tmp/cli_scandone.log"
#define SCAN_AP_RESULT_FILE "/tmp/cli_scanrlt.log"

static int start_scan_ap(int radio, char *result_file, char *done_flag_file)
{

	int rMode = 0;
	char cmd[128] = {0};
	char vap_name[8] = {0};
	char ModeTmpBuf[32] = {0};
	
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "ap")) {
			rMode = WLAN_MODE_AP;
		}
		else if (!strcmp(ModeTmpBuf, "client")) {
			rMode = WLAN_MODE_STA;
		}
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "ap")) {
			rMode = WLAN_MODE_AP;
		}
		else if (!strcmp(ModeTmpBuf, "client")) {
			rMode = WLAN_MODE_STA;
		}
	}
	else {
		fprintf(stderr, "ERROR:Radio error!\n");
        return T_FAILURE;
	}
	
	construct_vap(vap_name, radio, 0, rMode);
	
    /* foreground scan to speedup get scan result*/
    sprintf(cmd, "iwlist %s scanning fore | awk -f /etc/wl/rogueap.awk > %s && echo done >%s &", 
    vap_name, result_file, done_flag_file);
	//printf("%s\n", cmd);
    system(cmd);

    return 0;
}

static int parse_scan_ap_list(SCAN_AP_LIST *ap_list, char *result_file)
{
	FILE *fp;
	int ret = 0;
	int apNum = 0;
	int apNTWType = 0;
	int apChannel = 0;
	int apSignal = 0;
	int apEncryption = 0;
	int apAuthmode = 0;
	char cmd[128] = {0};
	char apSsid[64] = {0};
	char apWMode[8] = {0};
	char apBssid[32] = {0};
	char buffer[128] = {0}, *p_token = buffer, *token;

	//system("cat /tmp/cli_scanrlt.log");
    
    /*Set ap_num as 0 first*/
    ap_list->ap_num = 0;
    if (NULL == (fp = fopen(result_file,"r")))
    {
        fprintf(stderr, "Ap list is NONE\n");

        return T_FAILURE;        
    }
    else
    {	
    	lseek(fp,0,SEEK_SET);
        while(fgets(buffer, sizeof(buffer), fp)){
			memset(apBssid, 0x00, sizeof(apBssid));
			memset(apSsid, 0x00, sizeof(apSsid));
			memset(apWMode, 0x00, sizeof(apWMode));

			/* apnum | address | mode | channel | signal | athwmode | encryption | authMode | essid
			  * 03      26:FB:8D:00:67:BD       4       149     46      A/N     3       3       "wlan_1"
			  */
			//apnum
			token = strtok(buffer, ",");
			if(NULL != token)
				apNum = atoi(token);
			//address
			token = strtok(NULL, ",");
			if(NULL != token)
				strncpy(apBssid, token, 31);
			//mode
			token = strtok(NULL, ",");
			if(NULL != token)
				apNTWType = atoi(token);
			//channel
			token = strtok(NULL, ",");
			if(NULL != token)
				apChannel = atoi(token);
			//signal strength
			token = strtok(NULL, ",");
			if(NULL != token)
				apSignal = atoi(token);
			//apWMode
			token = strtok(NULL, ",");
			if(NULL != token)
				strncpy(apWMode, token, 7);
			//apEncryption
			token = strtok(NULL, ",");
			if(NULL != token)
				apEncryption = atoi(token);
			//apAuthmode
			token = strtok(NULL, ",");
			if(NULL != token)
				apAuthmode = atoi(token);
			//essid
			token = strtok(NULL, ",");
			if(NULL != token)
				strncpy(apSsid, token, 63);
			//printf("num: %d, bssid: %s, type: %d, channel: %d, signal: %d, mode: %s, encryption: %d, authmode: %d, ssid: %s\n",
				//apNum, apBssid, apNTWType, apChannel, apSignal, apWMode, apEncryption, apAuthmode, apSsid);

			memset(ap_list->ap[apNum - 1].ssid, 0x00, sizeof(ap_list->ap[apNum - 1].ssid));
            memset(ap_list->ap[apNum - 1].bssid, 0x00, sizeof(ap_list->ap[apNum - 1].bssid));
			memset(ap_list->ap[apNum - 1].rssi_str, 0x00, sizeof(ap_list->ap[apNum - 1].rssi_str));
			memset(ap_list->ap[apNum - 1].wmode, 0x00, sizeof(ap_list->ap[apNum - 1].wmode));

			/* AP BSSID */
			strcpy(ap_list->ap[apNum - 1].bssid, apBssid);

			/* AP Network Type: 0->Auto; 1->Ad-hoc; 2->Managed; 3->Repeater; 4->Master; 5->Secondary; 6->Monitor; 7->Unknow*/
			if (4 == apNTWType) {
				ap_list->ap[apNum - 1].network_type = 1;
			}
			else {
				ap_list->ap[apNum - 1].network_type = 0;
			}

			/* AP Channel*/
			ap_list->ap[apNum - 1].channel = apChannel;

			/* AP Signal*/
			ap_list->ap[apNum - 1].rssi = - apSignal;
			sprintf(ap_list->ap[apNum - 1].rssi_str, "%ddBm", ap_list->ap[apNum - 1].rssi);
			
			/* AP Wireless Mode*/
			strcpy(ap_list->ap[apNum - 1].wmode, apWMode);

			/* AP Encryption*/
			ap_list->ap[apNum -1].encry = apEncryption;

			/* AP Auth_mode*/
			ap_list->ap[apNum -1].auth_mode = apAuthmode;

			/* AP SSID*/
			strncpy(ap_list->ap[apNum -1].ssid, &apSsid[1], (strlen(apSsid) - 3));

			/* AP Number */
			ap_list->ap_num = apNum;
			if(ap_list->ap_num == 128)
            {
                break;
            }
        }
    }

    fclose(fp);

    sprintf(cmd, "rm -rf %s", result_file);
	system(cmd);

    return T_SUCCESS; 
}

int ap_list_cmp(const void*a, const void*b)
{
	return ((SCAN_AP_STATE*)a)[0].rssi < ((SCAN_AP_STATE*)b)[0].rssi;
}

char *gauth[] = {
	"None",
	"WPAPSK",
	"WPA2PSK",
	"WPA2-Mixed-PSK",
	"WPA",
	"WPA2",
	"WPA2-Mixed",
	"WEP"
};

char *gencry[] = {
	"None",
	"AES",
	"TKIP",
	//"IKIPAES",
	"AES/TKIP",
	"WEP"
};

int scanDeviceApHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int num = 0, i, j = 0;
	int NetworkType;
	char TMPssid[256], SSID[256];
	unsigned char tmpRadio[188], tmp_SSID[256], *p_ssid, *p_SSID, *tmpAuth, tmpEncry[16], tmpImg[40];
	unsigned char radiocheck[8];
	char *ssid_value;
	int max_wait = 0;
	FILE *fp=NULL;
	char cmd[128]={0};
	int ret =0;
	int assoc = 0;
	char assoc_ssid[256] = {0};
	char assoc_bssid[32] = {0};

	SCAN_AP_LIST *ap_list = (SCAN_AP_LIST *)malloc(sizeof(SCAN_AP_LIST));
	memset(ap_list, 0x00, sizeof(SCAN_AP_LIST));

    /* scan ap and write the result to /tmp/scandone*/
    start_scan_ap(0, SCAN_AP_RESULT_FILE, SCAN_AP_DONE_FILE);

	 /* wait for scan finished*/
    while(NULL == (fp = fopen(SCAN_AP_DONE_FILE,"r")))
    {
        sleep(2);

        max_wait++;

        if(max_wait == 30)
        {
            printf("%s: scan isn't finised!\n", __func__);
            break;
        }
    }

    /* delete scan done file*/
    if(fp)
    {
        fclose(fp);

        sprintf(cmd, "rm -fr %s", SCAN_AP_DONE_FILE);
        system(cmd);  
    }

	/* parse scan result*/
	if (parse_scan_ap_list(ap_list, SCAN_AP_RESULT_FILE) < 0) {
		free(ap_list);
		printf("get ap_list error\n");
		return 0;
	}

	qsort(&ap_list->ap, ap_list->ap_num,sizeof(ap_list->ap[0]), ap_list_cmp);

	if(ap_list->ap_num > 128)
	   ap_list->ap_num = 128;

	{
		int i = 0;
		int iarray[130] = { 0 };
		printf("Ssid\t\t\tBssid\t\t\tRssi\t\tChannel\tEncryption\tAuth\n");
		while (i < ap_list->ap_num) {
			memset(radiocheck, 0x00, sizeof(radiocheck));
			memset(tmpRadio, 0x00, sizeof(tmpRadio));
			memset(tmpImg, 0x00, sizeof(tmpImg));
			memset(TMPssid, 0x00, sizeof(TMPssid));

			int j;
			for (j = 0; j < sizeof(ap_list->ap[i].bssid); j++)
				ap_list->ap[i].bssid[j] = toupper(ap_list->ap[i].bssid[j]);

			if (ap_list->ap[i].auth_mode > 7) {
				printf("auth mode error %d\n", ap_list->ap[i].auth_mode);
			}
			tmpAuth = gauth[ap_list->ap[i].auth_mode];
			memset(tmpEncry, 0x00, sizeof(tmpEncry));
			if (ap_list->ap[i].encry > 4) {
				if ((ap_list->ap[i].encry > 33) || (ap_list->ap[i].encry < 10)) {
					sprintf(tmpEncry, "----");
					printf("encry error %d\n", ap_list->ap[i].encry);
				} else {
					sprintf(tmpEncry, "%s/%s", gencry[(ap_list->ap[i].encry / 10)], gencry[(ap_list->ap[i].encry % 10)]);
				}
			}
			else {
				sprintf(tmpEncry, gencry[ap_list->ap[i].encry]);
			}

			printf("%s\t\t\t%s\t%s\t\t%d\t%s\t%s\n", ap_list->ap[i].ssid, 
				ap_list->ap[i].bssid, ap_list->ap[i].rssi_str, 
				ap_list->ap[i].channel, tmpEncry, tmpAuth);

			i++;
		}
	}

	free(ap_list);

   /* 
     * Workaround:
     *   restart sta to fix connect is lost after scanning sometimes
     *   let child process to restart station
     */
    if(fork() == 0){
        set_config_sta(RADIO_2G);
        exit(0);
    }else
    {
        /* parent: do nothing*/
    }

	return T_SUCCESS; 
}
  /***********************************************************************
 * Function Name : factorydefaultSet
 * Description    : restore config to factory
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
 int factorydefaultSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
 {
 	system("nvram factory");
 	
	return CLI_PARSE_OK;
 }
 /***********************************************************************
 * Function Name : versionHandler
 * Description    : set firmware version
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int versionHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	FILE *fp;
	char c;
	char ch;
	int count=0,i=0;

	if ( tokenCount(pCli) > 0 ) {
		return CLI_PARSE_TOO_MANY;
	}

	if(!(fp=fopen("/version","r"))){
		uiPrintf("cant open file!\n");
		return T_FAILURE;
	}
	ch=fgetc(fp);
	while(ch!= EOF)
	{
		ch=fgetc(fp);
		count++;
	}
	fclose(fp);

	char *buffer = (char*)malloc(count*sizeof(char));
	if(buffer != NULL)
	{
		memset(buffer,0,count*sizeof(char));
	}
	if (!(fp = fopen("/version", "r")))
	{
		uiPrintf("cant open file!\n");
		free(buffer);
		return T_FAILURE;
	}
	while ((c = getc(fp)) != EOF) {
		//uiPrintf("%c",c);
		buffer[i] = c;
		i++;
	}
	i--;
	fclose(fp);
	buffer[i] = '\0';
	uiPrintf("Firmware Version %s\n",buffer);
	free(buffer);
	return CLI_PARSE_OK;
}

  /***********************************************************************
 * Function Name : applyIfAddress
 * Description    : used to config ip address
 * Input         : 
 * Output        : 
 * Return value  : T_SUCCESS, command success
 ***********************************************************************/
int applyIfAddress(void)
{
	//TODO:
	return T_SUCCESS;
}
  /***********************************************************************
 * Function Name : applyWlanBasicSettings
 * Description    : used to config basic wlan settings
 * Input         : 
 * Output        : 
 * Return value  : T_SUCCESS, command success
 ***********************************************************************/
int applyWlanBasicSettings(void)
{
	//TODO:Use nvram instead
	return T_SUCCESS;
}

