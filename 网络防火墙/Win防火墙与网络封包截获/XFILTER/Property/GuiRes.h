//=============================================================================================
/*
	GUI Resurce, GuiRes.h

	Project	: Xfilter 1.0
	Author	: Tony Zhu
	Create Date	: 2001/08/03
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/

#ifndef	GUIRES_H
#define GUIRES_H

#define GUI_LANGUAGE				_T("RPC")

//---------------------------------------------------------------------------------------------
//app's resource

#define GUI_APP_CLASS_NAME			_T("Xfilter")
#define	GUI_APP_CAPTION				_T("Xfilter 个人防火墙")
#define	GUI_APP_LOGO				_T("XSTUDIO")

//---------------------------------------------------------------------------------------------
//menu's resource

#define GUI_MENU_FILE				_T("文件(&F)")
#define GUI_MENU_ABOUT				_T("关于(&A)")
#define GUI_MENU_CONTROL_FRAME		_T("主窗口(&C)")
#define GUI_MENU_PACKET_MONITOR		_T("封包监视(&M)")
#define GUI_MENU_LOG_QUERY			_T("日志查询(&Q)")
#define GUI_MENU_ACL				_T("控管规则(&L)")
#define GUI_MENU_SYSTEM_SET			_T("系统设置(&S)")
#define GUI_MENU_STOP_FILTER		_T("停止过滤(&T)")
#define GUI_MENU_START_FILTER		_T("开始过滤(&F)")
#define GUI_MENU_EXIT				_T("退出(&X)")

//---------------------------------------------------------------------------------------------
//button's resource

#define GUI_BUTTON_OK				_T("确定")
#define GUI_BUTTON_CANCEL			_T("取消")
#define GUI_BUTTON_CANCEL_EX		_T("取消(&C)")
#define GUI_BUTTON_APPLY			_T("应用(&A)")
#define GUI_BUTTON_HELP				_T("帮助")

//---------------------------------------------------------------------------------------------
//tab's resource

#define GUI_TAB_PACKET_MONITOR		_T("封包监视")
#define GUI_TAB_LOG_QUERY			_T("日志查询")
#define GUI_TAB_ACL					_T("控管规则")
#define GUI_TAB_SYSTEM_SETTING		_T("系统设置")
#define GUI_TAB_ABOUT				_T("关于")

//---------------------------------------------------------------------------------------------
//packet monitor resource

#define	GUI_PACKET_MONITOR_TITLE						_T("封包监视")
#define GUI_PACKET_MONITOR_LABLE						_T("封包监视列表：")
#define GUI_PACKET_MONITOR_TOOLBAR_CLEAR				_T("清除")
#define GUI_PACKET_MONITOR_TOOLBAR_STOP_SCROLL			_T("停止滚动")
#define GUI_PACKET_MONITOR_TOOLBAR_START_SCROLL			_T("自动滚动")
#define GUI_PACKET_MONITOR_TOOLBAR_STOP_MONITOR			_T("停止监视")
#define GUI_PACKET_MONITOR_TOOLBAR_START_MONITOR		_T("开始监视")
#define GUI_PACKET_MONITOR_LIST_ACTION					_T("动作")
#define GUI_PACKET_MONITOR_LIST_ACTION_SIZE				\
		sizeof(GUI_PACKET_MONITOR_LIST_ACTION)			* 8
#define GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME		_T("开始时间 - 结束时间")
#define GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME_SIZE	\
		sizeof(GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME) * 8
#define GUI_PACKET_MONITOR_LIST_PROTOCOL				_T("协议")
#define GUI_PACKET_MONITOR_LIST_PROTOCOL_SIZE		\
		sizeof(GUI_PACKET_MONITOR_LIST_PROTOCOL)		* 8
#define GUI_PACKET_MONITOR_LIST_IN_DATA					_T("进流量")
#define GUI_PACKET_MONITOR_LIST_IN_DATA_SIZE			\
		sizeof(GUI_PACKET_MONITOR_LIST_IN_DATA)			* 8
#define GUI_PACKET_MONITOR_LIST_OUT_DATA				_T("出流量")
#define GUI_PACKET_MONITOR_LIST_OUT_DATA_SIZE			\
		sizeof(GUI_PACKET_MONITOR_LIST_OUT_DATA)		* 8
#define GUI_PACKET_MONITOR_LIST_IP_PORT					_T("本地 IP : 端口 - 远端 IP : 端口")
#define GUI_PACKET_MONITOR_LIST_IP_PORT_SIZE			\
		sizeof(GUI_PACKET_MONITOR_LIST_IP_PORT)			* 8
#define GUI_PACKET_MONITOR_LIST_APPLICATION				_T("应用程序 : 备注")
#define GUI_PACKET_MONITOR_LIST_APPLICATION_SIZE		\
		sizeof(GUI_PACKET_MONITOR_LIST_APPLICATION)		* 8

//---------------------------------------------------------------------------------------------
//log query resource

#define	GUI_LOG_QUERY_SET_LABLE				_T("查询条件设置")
#define GUI_LOG_QUERY_SET_START_TIME_LABLE	_T("开始日期/时间：")
#define GUI_LOG_QUERY_SET_END_TIME_LABLE	_T("结束日期/时间：")
#define GUI_LOG_QUERY_RESULT_LABLE			_T("查询结果")
#define GUI_LOG_QUERY_RESULT_LIST_LABLE		_T("第 %u 页,共 %u 页,每页 %u 笔,当前 %u 笔,共 %u 笔")
#define GUI_LOG_QUERY_BUTTON_QUERY			_T("开始查询(&Q)")
#define GUI_LOG_QUERY_BUTTON_BACK			_T("上一页(&B)")
#define GUI_LOG_QUERY_BUTTON_NEXT			_T("下一页(&N)")
#define GUI_LOG_QUERY_BUTTON_DELETE			_T("清除(&D)")

//---------------------------------------------------------------------------------------------
//ACL RESOURCE

#define GUI_ACL_APPLICATION_SET_LABLE		_T("应用程序设置")
#define GUI_ACL_RADIO_PASS					_T("放行所有")
#define GUI_ACL_RADIO_QUERY					_T("询问")
#define GUI_ACL_RADIO_DENY					_T("拒绝所有")
#define GUI_ACL_SET_LABLE					_T("控管规则设置")
#define GUI_ACL_BUTTON_ADD					_T("添加(&A)")
#define GUI_ACL_BUTTON_EDIT					_T("修改(&E)")
#define GUI_ACL_BUTTON_DELETE				_T("删除(&D)")
#define GUI_ACL_LIST_ACTION					_T("动作")
#define GUI_ACL_LIST_ACTION_SIZE			sizeof(GUI_ACL_LIST_ACTION)	* 8
#define GUI_ACL_LIST_APPLICATION			_T("应用程序")
#define GUI_ACL_LIST_APPLICATION_SIZE		sizeof(GUI_ACL_LIST_APPLICATION) * 8
#define GUI_ACL_LIST_DIRECTION				_T("方向")
#define GUI_ACL_LIST_DIRECTION_SIZE			sizeof(GUI_ACL_LIST_DIRECTION) * 8
#define GUI_ACL_LIST_REMOTE_NET				_T("远端网络")
#define GUI_ACL_LIST_REMOTE_NET_SIZE		sizeof(GUI_ACL_LIST_REMOTE_NET)	* 8
#define GUI_ACL_LIST_ACCESS_TIME			_T("访问时间")
#define GUI_ACL_LIST_ACCESS_TIME_SIZE		sizeof(GUI_ACL_LIST_ACCESS_TIME) * 8
#define GUI_ACL_LIST_TYPE_PORT				_T("类型/端口")
#define GUI_ACL_LIST_TYPE_PORT_SIZE			sizeof(GUI_ACL_LIST_TYPE_PORT) * 8
#define GUI_ACL_LIST_APPLICATION_PATH		_T("应用程序路径")
#define GUI_ACL_LIST_APPLICATION_PATH_SIZE	sizeof(GUI_ACL_LIST_APPLICATION_PATH) * 8
#define GUI_ACL_LIST_MEMO					_T("说明")
#define GUI_ACL_LIST_MEMO_SIZE				sizeof(GUI_ACL_LIST_MEMO) * 8

//---------------------------------------------------------------------------------------------
//ACL SET RESOURCE

#define GUI_ACL_SET_CAPTION					_T("Xfilter 个人防火墙 - 控管规则")
#define GUI_ACL_BASE_SET_LABLE				_T("基本设置")
#define GUI_ACL_QUERY_INFO					_T("要访问网络，放行吗？")
#define GUI_ACL_APPLICATION_LABLE			_T("应用程序：")
#define GUI_ACL_REMOTE_NET_LABLE			_T("远端网络：")
#define GUI_ACL_ACCESS_TIME_LABLE			_T("访问时间：")
#define GUI_ACL_ACTION_LABLE				_T("管制动作：")
#define GUI_ACL_SUPER_SET_LABLE				_T("高级设置：服务端口为 0 表示所有端口。")
#define GUI_ACL_DERECTION_LABLE				_T("进出方向：")
#define GUI_ACL_SERVICE_TYPE_LABLE			_T("服务类型：")
#define GUI_ACL_SERVICE_PORT_LABLE			_T("服务端口：")
#define GUI_ACL_MEMO_LABLE					_T("备注")
#define GUI_ACL_ACTION_0					_T("放行")
#define GUI_ACL_ACTION_1					_T("拒绝")
#define GUI_ACL_DERECTION_0					_T("进")
#define GUI_ACL_DERECTION_1					_T("出")
#define GUI_ACL_DERECTION_2					_T("双向")
#define GUI_ACL_SERVICE_TYPE_0				_T("全部")
#define GUI_ACL_SERVICE_TYPE_1				_T("TCP")
#define GUI_ACL_SERVICE_TYPE_2				_T("UDP")
#define GUI_ACL_SERVICE_TYPE_3				_T("FTP")
#define GUI_ACL_SERVICE_TYPE_4				_T("TELNET")
#define GUI_ACL_SERVICE_TYPE_5				_T("HTTP")
#define GUI_ACL_SERVICE_TYPE_6				_T("NNTP")
#define GUI_ACL_SERVICE_TYPE_7				_T("POP3")
#define GUI_ACL_SERVICE_TYPE_8				_T("SMTP")

//---------------------------------------------------------------------------------------------
//net and time set sheet resource

#define GUI_NET_TIME_SHEET_CAPTION			_T("网络与时间设置")

//---------------------------------------------------------------------------------------------
//set time resource

#define GUI_SET_TIME_WEEK_TIME_LABLE		_T("星期与时间")
#define GUI_SET_TIME_CHECK_SUNDAY			_T("星期日")
#define GUI_SET_TIME_CHECK_MONDAY			_T("星期一")
#define GUI_SET_TIME_CHECK_TUESDAY			_T("星期二")
#define GUI_SET_TIME_CHECK_WEDNESDAY		_T("星期三")
#define GUI_SET_TIME_CHECK_THURSDAY			_T("星期四")
#define GUI_SET_TIME_CHECK_FRIDAY			_T("星期五")
#define GUI_SET_TIME_CHECK_SATURDAY			_T("星期六")
#define GUI_SET_TIME_TIME_LABLE				_T("时间：")
#define GUI_SET_TIME_LABLE_START_TIME		_T("开始时间")
#define GUI_SET_TIME_LABLE_END_TIME			_T("结束时间")
#define GUI_SET_TIME_TREE_0					_T("任何时间")
#define GUI_SET_TIME_TREE_1					_T("工作时间")
#define GUI_SET_TIME_TREE_2					_T("工作日业余时间")
#define GUI_SET_TIME_TREE_3					_T("周末")
#define GUI_SET_TIME_TREE_4					_T("约束时间")
#define GUI_SET_TIME_TREE_5					_T("信任时间")
#define GUI_SET_TIME_TREE_6					_T("自定义时间")

//---------------------------------------------------------------------------------------------
//net ip aria resource

#define GUI_NET_IP_ARIA_CAPTION				_T("IP 地址段")
#define GUI_NET_IP_ARIA_LABLE				_T("远端网络 IP 地址段")
#define GUI_NET_IP_ARIA_LABLE_START_IP		_T("开始 IP：")
#define GUI_NET_IP_ARIA_LABLE_END_IP		_T("结束 IP：")
#define GUI_NET_IP_ARIA_ALL					_T("*")

//---------------------------------------------------------------------------------------------
//set remote net resource

#define GUI_SET_NET_LABLE					_T("远端网络信息")
#define GUI_SET_NET_NET_LABLE				_T("网络信息")
#define GUI_SET_NET_BUTTON_ADD				_T("添加(&A)")
#define GUI_SET_NET_BUTTON_EDIT				_T("修改(&E)")
#define GUI_SET_NET_BUTTON_DELETE			_T("删除(&D)")
#define GUI_SET_NET_LIST_START_IP			_T("开始IP")
#define GUI_SET_NET_LIST_END_IP				_T("结束IP")
#define GUI_SET_NET_TREE_0					_T("所有网络")
#define GUI_SET_NET_TREE_1					_T("局域网")
#define GUI_SET_NET_TREE_2					_T("受约束的网络")
#define GUI_SET_NET_TREE_3					_T("信任的网络")
#define GUI_SET_NET_TREE_4					_T("自定义的网络")

//---------------------------------------------------------------------------------------------
//system set resource

#define GUI_SYSTEM_SET_COMMON_SET_LABLE		_T("公共设置")
#define GUI_SYSTEM_SET_CHECK_LOG			_T("记录日志，日志文件大小为")
#define GUI_SYSTEM_SET_UNIT_LABLE			_T("M。")
#define GUI_SYSTEM_SET_CHECK_AUTOSTART		_T("Windows 启动时自动启动 Xfilter。")
#define GUI_SYSTEM_SET_CHECK_SPLASH			_T("Xfilter 启动时显示欢迎界面。")
#define GUI_SYSTEM_SET_ALERT_SET_LABLE		_T("报警设置")
#define GUI_SYSTEM_SET_CHECK_ALERT_PCSPEAKER	_T("拦截时 PC 喇叭发出报警声音。")
#define GUI_SYSTEM_SET_CHECK_ALERT_DIALOG	_T("拦截时闪烁任务栏上的图标。")

//---------------------------------------------------------------------------------------------
//about resource

#define GUI_ABOUT_LABLE_TITLE			_T("关于 Xfilter 个人防火墙")
#define GUI_ABOUT_LABLE_COPYRIGHT1		_T("Xfilter 个人防火墙 %u.%u.%u 版")
#define GUI_ABOUT_LABLE_COPYRIGHT2		_T("Copyright (C) 2001-2002 Xstudio Technology")
#define GUI_ABOUT_LABLE_COPYRIGHT3		_T("All rights reserved.")
#define GUI_ABOUT_LABLE_ACCREDIT_TO		_T("本软件授权给 %s 使用")
#define GUI_ABOUT_LABLE_WEB_ADDRESS_LABLE	_T("网站地址：")
#define GUI_ABOUT_LABLE_EMAIL_LABLE			_T("电子邮件：")
#define GUI_ABOUT_LABLE_WEB_ADDRESS		_T("http://www.xfilt.com/")
#define GUI_ABOUT_LABLE_EMAIL			_T("mailto:xstudio@xfilt.com")
#define GUI_ABOUT_LABLE_WARNING			_T("警告：本软件受著作权法的保护。允许完整的自由传播，但不允许破坏软件的完整性或者作为盈利目的，凡未经授权而进行营利活动和其它一切商业行为，将遭到民事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。")
#define GUI_ABOUT_LABLE_INFO			_T("此软件出售源代码，详细信息请浏览上面的网址。网站上列出了产品价格和付款方式，或者直接发送 Email 进行咨询。")
#define GUI_ABOUT_LABLE_AUTHOR			_T("作者：    朱雁辉、朱雁冰")
#define GUI_ABOUT_LABLE_COPYRIGHT		_T("版权所有(C) 2001-2002 X 工作室")

//---------------------------------------------------------------------------------------------
// Register Dialog Resource

#define GUI_REG_CAPTION				_T("Xfilter 用户注册")
#define GUI_REG_LABEL_INFO_1		_T("本软件出售源代码，详细信息请浏览下面的网址。软件本身是完全免费的，只要输入 Email 地址，您就成为本软件的合法用户。")
#define GUI_REG_LABEL_INFO_3		_T("请确保 Email 的真实性，这将作为技术支持的主要途径。")
#define GUI_REG_LABEL_INFO_2		_T("注册以下信息您将可以在第一时间得到本软件的升级程序，并有可能成为幸运用户免费获得源代码。 ")
#define GUI_REG_LABEL_EMAIL			_T("Email：*")
#define GUI_REG_LABEL_NAME			_T("姓名：")
#define GUI_REG_LABEL_GENDER		_T("性别：")
#define GUI_REG_LABEL_ID_TYPE		_T("证件类型：")
#define GUI_REG_LABEL_ID			_T("证件号码：")
#define GUI_REG_LABEL_BIRTHDAY		_T("生日：")
#define GUI_REG_LABEL_DEGREE		_T("学历：")
#define GUI_REG_LABEL_METIER		_T("职业：")
#define GUI_REG_LABEL_DUTY			_T("职务：")
#define GUI_REG_LABEL_ZIP			_T("邮编：")
#define GUI_REG_LABEL_SALARY		_T("月薪：")
#define GUI_REG_LABEL_ADDRESS		_T("地址：")
#define GUI_REG_LABEL_QQ			_T("QQ 号码：")
#define GUI_REG_LABEL_INC			_T("单位：")

//---------------------------------------------------------------------------------------------
// Popup Message

#define GUI_REMOVE_CAPTION				_T("卸载 Xfilter 个人防火墙")
#define GUI_REMOVE_LABEL_INFO			_T("卸载程序将从您计算机上移除 Xfilter 个人防火墙，您可以定制要卸载的附加组件，按确定按钮开始卸载，按取消按钮取消卸载。")
#define GUI_REMOVE_CHECK_MAIN			_T("卸载 Xfilter 个人防火墙主程序")
#define GUI_REMOVE_CHECK_LOG			_T("卸载时删除日志文件")
#define GUI_REMOVE_CHECK_ACL			_T("卸载时删除控管规则文件")
#define GUI_REMOVE_LABEL_LOGO			_T("XSTUDIO")

//---------------------------------------------------------------------------------------------
// Popup Message

#define GUI_ACL_MESSAGE_INAVALID_PORT					_T("端口值无效，有效范围为 0 - 65535，请重新输入。")
#define GUI_ACL_MESSAGE_APP_PATH_ERROR					_T("必须输入应用程序路径和名称，请重新输入。")
#define GUI_ACL_MESSAGE_APP_NOT_EXSITS					_T("应用程序不存在，请检查路径和名称。")
#define GUI_ACL_MESSAGE_ONLY_PATH						_T("必须输入应用程序完整的路径和文件名。")
#define GUI_ACL_MESSAGE_ADD_ACL_IP_ERROR				_T("添加 ACL IP范围错误。")
#define GUI_ACL_MESSAGE_SAVE_ACL_IP_TIME_ERROR			_T("保存 ACL IP范围或者时间范围错误。")
#define GUI_ACL_MESSAGE_ADD_ACL_ERROR					_T("添加 ACL 规则错误。")
#define GUI_ACL_MESSAGE_SAVE_ACL_ERROR					_T("保存 ACL 规则错误。")
#define GUI_ACL_MESSAGE_ACL_FILE_SET_ERROR				_T("ACL 文件系统设置错误。")
#define GUI_ACL_MESSAGE_ACL_READ_ERROR					_T("读取 ACL 文件错误。")
#define GUI_ACL_MESSAGE_ACL_ASK_SAVE					_T("要保存所做的修改吗？")
#define GUI_ACL_MESSAGE_PLEASE_CLOSE_SUB_WINDOW			_T("请首先关闭所有的子窗口。")
#define GUI_ACL_MESSAGE_MAX_ACL							_T("超过 ACL 规则的最大数目，不能够再添加。")
#define GUI_ACL_MESSAGE_MAX_IP_ARIA						_T("超过 IP 范围的最大数目，不能够再添加。")
#define GUI_ACL_MESSAGE_DLL_NOT_FOUND					_T("不能找到 Xfilter.dll 或者访问被拒绝。")
#define GUI_ACL_MESSAGE_FUNCTION_NOT_FOUNT				_T("Xfilter.dll 中没有找到成员函数。")
#define GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR				_T("初始化 Xfilter.dll 数据区错误。")
#define GUI_ACL_MESSAGE_INIT_CALLBACK_FUNC_ERROR		_T("初始化回调函数错误。")
#define GUI_NET_IP_ARIA_MESSAGE_IP_NULL					_T("IP地址不能为空，请重新输入。")
#define GUI_NET_IP_ARIA_MESSAGE_INVALID_IP_ARIA			_T("开始 IP 必须小于等于结束 IP，请重新输入。")
#define GUI_ACL_MESSAGE_START_TIME_MIN_END_TIME			_T("开始时间必须小于结束时间。")
#define GUI_ACL_MESSAGE_CAN_NOT_FIND_RECORD				_T("没有找到符合条件的记录。")
#define GUI_ACL_MESSAGE_SET_WORK_MODE_ERROR				_T("设置工作模式错误。")
#define GUI_REG_MESSAGE_MUST_INPUT_EMAIL				_T("请输入正确的 Email 地址。")
#define GUI_REG_MESSAGE_INVALID_PASSWORD				_T("无效的密码，请重新输入。")
#define GUI_REG_MESSAGE_MUST_INPUT_NAME					_T("必须输入姓名。")
#define GUI_REG_MESSAGE_MUST_INPUT_ID					_T("必须输入证件号码。")
#define GUI_ACL_MESSAGE_NO_WINSOCK2						_T("系统没有 Winsock 2 支持。")
#define GUI_ACL_MESSAGE_INSTALL_FAILED					_T("安装失败。")
#define GUI_ACL_MESSAGE_PROCESS_IS_RUNNING				_T("Xfilter 正在运行，卸载前请首先关闭 Xfilter。")
#define GUI_MESSAGE_REMOVE_PROVIDER_FAILED				_T("卸载 Xfilter 服务程序错误，Xfilter没有安装或者读写注册表错误。")
#define GUI_MESSAGE_REMOVE_SUCCESS						_T("卸载成功。")
#define GUI_MESSAGE_REMOVE_HAVE_FILE_USING				_T("卸载成功，但是有文件正在使用，请重新启动后手工删除。")
#define GUI_MESSAGE_OPEN_HELP_FAILED					_T("打开帮助文件错误。")
#define GUI_MESSAGE_HYPER_LINK_ERROR					_T("打开超级链接出错。")

//---------------------------------------------------------------------------------------------
// some resource arrays

static TCHAR *GUI_ACTION[]		= {
	GUI_ACL_ACTION_0,
	GUI_ACL_ACTION_1
};

static TCHAR *GUI_DIRECTION[]	= {
	GUI_ACL_DERECTION_0,
	GUI_ACL_DERECTION_1,
	GUI_ACL_DERECTION_2
};

static TCHAR *GUI_DIRECTION_EX[]= {
	_T("<-"),
	_T("->"),
	_T("<->")
};

static TCHAR *GUI_SERVICE_TYPE[]= {
	 GUI_ACL_SERVICE_TYPE_0,	
	 GUI_ACL_SERVICE_TYPE_1,						
	 GUI_ACL_SERVICE_TYPE_2,						
	 GUI_ACL_SERVICE_TYPE_3,						
	 GUI_ACL_SERVICE_TYPE_4,						
	 GUI_ACL_SERVICE_TYPE_5,						
	 GUI_ACL_SERVICE_TYPE_6,						
	 GUI_ACL_SERVICE_TYPE_7,						
	 GUI_ACL_SERVICE_TYPE_8	
};

static TCHAR *GUI_TIME[]		= {
	GUI_SET_TIME_TREE_0,
	GUI_SET_TIME_TREE_1,
	GUI_SET_TIME_TREE_2,
	GUI_SET_TIME_TREE_3,
	GUI_SET_TIME_TREE_4,
	GUI_SET_TIME_TREE_5,
	GUI_SET_TIME_TREE_6
};

static TCHAR *GUI_NET[]			= {
	GUI_SET_NET_TREE_0,
	GUI_SET_NET_TREE_1,
	GUI_SET_NET_TREE_2,
	GUI_SET_NET_TREE_3,
	GUI_SET_NET_TREE_4
};

static TCHAR *GUI_GENDER[]		= {
	_T("男"),
	_T("女")
};

static TCHAR *GUI_ID_TYPE[]		= {
	_T("其它"),
	_T("身份证"),
	_T("学生证"),
	_T("军官证")
};

static TCHAR *GUI_DEGREE[]		= {
	_T("其它"),
	_T("小学"),
	_T("初中"),
	_T("高中"),
	_T("中专"),
	_T("大专"),
	_T("大学"),
	_T("硕士"),
	_T("博士")
};

static TCHAR *GUI_METIER[]		= {
	_T("其它"),
	_T("计算机业"),
	_T("学生"),
	_T("教师"),
	_T("军人"),
	_T("娱乐业"),
	_T("商业"),
	_T("农业"),
	_T("服务业"),
	_T("制造业"),
	_T("金融业"),
	_T("广告业"),
	_T("销售"),
	_T("法律"),
	_T("政府部门"),
	_T("医疗")
};

static TCHAR *GUI_SALARY[]		= {
	_T("无"),
	_T("1000 以下"),
	_T("1000 - 2500"),
	_T("2500 - 4000"),
	_T("4000 - 6000"),
	_T("6000 - 8000"),
	_T("8000 - 10000"),
	_T("10000 以上")
};

#endif

