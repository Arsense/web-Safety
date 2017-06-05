VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TabCtl32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "XFILTER 个人防火墙"
   ClientHeight    =   5895
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8775
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   5895
   ScaleWidth      =   8775
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton cmdHelp 
      Caption         =   "帮助(&H)"
      Height          =   300
      Left            =   7635
      TabIndex        =   53
      Top             =   5520
      Width           =   1050
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "确定"
      Default         =   -1  'True
      Height          =   300
      Left            =   4245
      TabIndex        =   3
      Top             =   5520
      Width           =   1050
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "取消"
      Height          =   300
      Left            =   5375
      TabIndex        =   2
      Top             =   5520
      Width           =   1050
   End
   Begin VB.CommandButton cmdUsed 
      Caption         =   "应用(&A)"
      Height          =   300
      Left            =   6505
      TabIndex        =   1
      Top             =   5520
      Width           =   1050
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   5385
      Left            =   45
      TabIndex        =   0
      Top             =   75
      Width           =   8670
      _ExtentX        =   15293
      _ExtentY        =   9499
      _Version        =   393216
      Style           =   1
      Tabs            =   5
      TabsPerRow      =   5
      TabHeight       =   520
      TabCaption(0)   =   "封包监视"
      TabPicture(0)   =   "frmMain.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "lblLog(0)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "imgLog"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "lblLog(1)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "tbLog"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "ListView2"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Frame1"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).ControlCount=   6
      TabCaption(1)   =   "日志查询"
      TabPicture(1)   =   "frmMain.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Toolbar3"
      Tab(1).Control(1)=   "ListView3"
      Tab(1).Control(2)=   "Frame3"
      Tab(1).Control(3)=   "Text2"
      Tab(1).Control(4)=   "Text1"
      Tab(1).Control(5)=   "Frame2"
      Tab(1).Control(6)=   "Toolbar2"
      Tab(1).Control(7)=   "lblLog(6)"
      Tab(1).Control(8)=   "Image2"
      Tab(1).Control(9)=   "lblLog(5)"
      Tab(1).Control(10)=   "lblLog(4)"
      Tab(1).Control(11)=   "lblLog(3)"
      Tab(1).Control(12)=   "Image1"
      Tab(1).Control(13)=   "lblLog(2)"
      Tab(1).ControlCount=   14
      TabCaption(2)   =   "控管规则"
      TabPicture(2)   =   "frmMain.frx":0038
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "frmApp"
      Tab(2).Control(1)=   "picApp"
      Tab(2).Control(2)=   "famAcl"
      Tab(2).Control(3)=   "Toolbar1"
      Tab(2).Control(4)=   "ListView1"
      Tab(2).Control(5)=   "lblApp"
      Tab(2).Control(6)=   "imgApp"
      Tab(2).Control(7)=   "lblAcl(0)"
      Tab(2).Control(8)=   "imgAcl"
      Tab(2).Control(9)=   "lblAcl(1)"
      Tab(2).ControlCount=   10
      TabCaption(3)   =   "系统设置"
      TabPicture(3)   =   "frmMain.frx":0054
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "Check3"
      Tab(3).Control(1)=   "Check2(1)"
      Tab(3).Control(2)=   "Check2(0)"
      Tab(3).Control(3)=   "Frame5"
      Tab(3).Control(4)=   "Check1"
      Tab(3).Control(5)=   "List1"
      Tab(3).Control(6)=   "Frame4"
      Tab(3).Control(7)=   "Label4"
      Tab(3).Control(8)=   "Image4"
      Tab(3).Control(9)=   "Label3"
      Tab(3).Control(10)=   "Label2"
      Tab(3).Control(11)=   "Image3"
      Tab(3).Control(12)=   "Label1"
      Tab(3).ControlCount=   13
      TabCaption(4)   =   "关于"
      TabPicture(4)   =   "frmMain.frx":0070
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame7"
      Tab(4).Control(1)=   "Frame6"
      Tab(4).Control(2)=   "Label12"
      Tab(4).Control(3)=   "Label11"
      Tab(4).Control(4)=   "Label10"
      Tab(4).Control(5)=   "Image6"
      Tab(4).Control(6)=   "Label9"
      Tab(4).Control(7)=   "Label8"
      Tab(4).Control(8)=   "Label7"
      Tab(4).Control(9)=   "Label6"
      Tab(4).Control(10)=   "Label5"
      Tab(4).Control(11)=   "Image5"
      Tab(4).ControlCount=   12
      Begin VB.Frame Frame7 
         Height          =   90
         Left            =   -74940
         TabIndex        =   49
         Top             =   3405
         Width           =   8565
      End
      Begin VB.Frame Frame6 
         Height          =   90
         Left            =   -74940
         TabIndex        =   43
         Top             =   1005
         Width           =   8565
      End
      Begin VB.CheckBox Check3 
         Caption         =   "记录日志，"
         Height          =   255
         Left            =   -73185
         TabIndex        =   42
         Top             =   1215
         Width           =   1200
      End
      Begin VB.CheckBox Check2 
         Caption         =   "拦截时弹出对话框报警。"
         Height          =   255
         Index           =   1
         Left            =   -73185
         TabIndex        =   41
         Top             =   3150
         Width           =   3180
      End
      Begin VB.CheckBox Check2 
         Caption         =   "拦截时PC喇叭发出声音报警。"
         Height          =   255
         Index           =   0
         Left            =   -73185
         TabIndex        =   40
         Top             =   2835
         Width           =   3180
      End
      Begin VB.Frame Frame5 
         Height          =   90
         Left            =   -74925
         TabIndex        =   38
         Top             =   2460
         Width           =   8565
      End
      Begin VB.CheckBox Check1 
         Caption         =   "系统启动时自动启动。"
         Height          =   255
         Left            =   -73185
         TabIndex        =   37
         Top             =   1575
         Width           =   3180
      End
      Begin VB.ListBox List1 
         Height          =   240
         Left            =   -70830
         TabIndex        =   35
         Top             =   1245
         Width           =   855
      End
      Begin VB.Frame Frame4 
         Height          =   90
         Left            =   -74940
         TabIndex        =   32
         Top             =   930
         Width           =   8565
      End
      Begin MSComctlLib.Toolbar Toolbar3 
         Height          =   330
         Left            =   -69450
         TabIndex        =   31
         Top             =   2115
         Width           =   2970
         _ExtentX        =   5239
         _ExtentY        =   582
         ButtonWidth     =   1667
         ButtonHeight    =   582
         Style           =   1
         TextAlignment   =   1
         _Version        =   393216
         BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
            NumButtons      =   3
            BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "上一页"
               Key             =   "back"
            EndProperty
            BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "下一页"
               Key             =   "next"
            EndProperty
            BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "删除"
               Key             =   "delete"
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.ListView ListView3 
         Height          =   2865
         Left            =   -74955
         TabIndex        =   30
         Top             =   2460
         Width           =   8565
         _ExtentX        =   15108
         _ExtentY        =   5054
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   0
      End
      Begin VB.Frame Frame3 
         Height          =   90
         Left            =   -74955
         TabIndex        =   28
         Top             =   1995
         Width           =   8565
      End
      Begin VB.TextBox Text2 
         Height          =   270
         Left            =   -69480
         TabIndex        =   27
         Text            =   "Text1"
         Top             =   1170
         Width           =   2460
      End
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   -73410
         TabIndex        =   26
         Text            =   "Text1"
         Top             =   1185
         Width           =   2460
      End
      Begin VB.Frame Frame2 
         Height          =   90
         Left            =   -74970
         TabIndex        =   21
         Top             =   945
         Width           =   8565
      End
      Begin VB.Frame Frame1 
         Height          =   90
         Left            =   45
         TabIndex        =   17
         Top             =   810
         Width           =   8565
      End
      Begin MSComctlLib.ListView ListView2 
         Height          =   3990
         Left            =   45
         TabIndex        =   16
         Top             =   1335
         Width           =   8580
         _ExtentX        =   15134
         _ExtentY        =   7038
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   0
      End
      Begin VB.Frame frmApp 
         Height          =   90
         Left            =   -74940
         TabIndex        =   11
         Top             =   975
         Width           =   8565
      End
      Begin VB.PictureBox picApp 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         ForeColor       =   &H80000008&
         Height          =   495
         Left            =   -74820
         ScaleHeight     =   495
         ScaleWidth      =   6465
         TabIndex        =   7
         Top             =   1155
         Width           =   6465
         Begin VB.OptionButton optApp 
            Caption         =   "拒绝所有"
            Height          =   240
            Index           =   2
            Left            =   4455
            TabIndex        =   10
            Top             =   135
            Width           =   1365
         End
         Begin VB.OptionButton optApp 
            Caption         =   "放行所有"
            Height          =   240
            Index           =   0
            Left            =   360
            TabIndex        =   9
            Top             =   135
            Width           =   1365
         End
         Begin VB.OptionButton optApp 
            Caption         =   "询问"
            Height          =   240
            Index           =   1
            Left            =   2400
            TabIndex        =   8
            Top             =   135
            Value           =   -1  'True
            Width           =   1365
         End
      End
      Begin VB.Frame famAcl 
         Height          =   90
         Left            =   -74940
         TabIndex        =   6
         Top             =   2160
         Width           =   8565
      End
      Begin MSComctlLib.Toolbar Toolbar1 
         Height          =   330
         Left            =   -68985
         TabIndex        =   4
         Top             =   2325
         Width           =   2565
         _ExtentX        =   4524
         _ExtentY        =   582
         ButtonWidth     =   1349
         ButtonHeight    =   582
         Style           =   1
         TextAlignment   =   1
         _Version        =   393216
         BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
            NumButtons      =   3
            BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "增加"
               Key             =   "add"
            EndProperty
            BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "修改"
               Key             =   "edit"
            EndProperty
            BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "删除"
               Key             =   "delete"
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.ListView ListView1 
         Height          =   2655
         Left            =   -74955
         TabIndex        =   5
         Top             =   2670
         Width           =   8565
         _ExtentX        =   15108
         _ExtentY        =   4683
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   0
      End
      Begin MSComctlLib.Toolbar Toolbar2 
         Height          =   330
         Left            =   -68115
         TabIndex        =   20
         Top             =   1590
         Width           =   1170
         _ExtentX        =   2064
         _ExtentY        =   582
         ButtonWidth     =   1984
         ButtonHeight    =   582
         Style           =   1
         TextAlignment   =   1
         _Version        =   393216
         BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
            NumButtons      =   1
            BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "开始查询"
               Key             =   "query"
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.Toolbar tbLog 
         Height          =   330
         Left            =   5910
         TabIndex        =   54
         Top             =   1005
         Width           =   2670
         _ExtentX        =   4710
         _ExtentY        =   582
         ButtonWidth     =   1984
         ButtonHeight    =   582
         Style           =   1
         TextAlignment   =   1
         _Version        =   393216
         BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
            NumButtons      =   2
            BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "停止滚动"
               Key             =   "scroll"
            EndProperty
            BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
               Caption         =   "停止监视"
               Key             =   "monitor"
            EndProperty
         EndProperty
      End
      Begin VB.Label lblLog 
         Caption         =   "封包监视列表："
         Height          =   210
         Index           =   1
         Left            =   90
         TabIndex        =   55
         Top             =   1050
         Width           =   2445
      End
      Begin VB.Label Label12 
         Caption         =   "网站："
         Height          =   210
         Left            =   -74775
         TabIndex        =   52
         Top             =   3990
         Width           =   1080
      End
      Begin VB.Label Label11 
         Caption         =   "作者："
         Height          =   210
         Left            =   -74775
         TabIndex        =   51
         Top             =   3645
         Width           =   1080
      End
      Begin VB.Label Label10 
         Caption         =   "关于 其它信息"
         Height          =   210
         Left            =   -74130
         TabIndex        =   50
         Top             =   3060
         Width           =   4755
      End
      Begin VB.Image Image6 
         Height          =   480
         Left            =   -74805
         Picture         =   "frmMain.frx":008C
         Top             =   2865
         Width           =   480
      End
      Begin VB.Label Label9 
         Caption         =   "声明："
         Height          =   210
         Left            =   -74775
         TabIndex        =   48
         Top             =   2160
         Width           =   1080
      End
      Begin VB.Label Label8 
         Caption         =   "版权："
         Height          =   210
         Left            =   -74775
         TabIndex        =   47
         Top             =   1860
         Width           =   1080
      End
      Begin VB.Label Label7 
         Caption         =   "授权："
         Height          =   210
         Left            =   -74775
         TabIndex        =   46
         Top             =   1590
         Width           =   1080
      End
      Begin VB.Label Label6 
         Caption         =   "版本："
         Height          =   210
         Left            =   -74775
         TabIndex        =   45
         Top             =   1290
         Width           =   1080
      End
      Begin VB.Label Label5 
         Caption         =   "关于 XFILTER 个人防火墙"
         Height          =   210
         Left            =   -74130
         TabIndex        =   44
         Top             =   660
         Width           =   4755
      End
      Begin VB.Image Image5 
         Height          =   480
         Left            =   -74805
         Picture         =   "frmMain.frx":04CE
         Top             =   465
         Width           =   480
      End
      Begin VB.Label Label4 
         Caption         =   "报警设置"
         Height          =   210
         Left            =   -74115
         TabIndex        =   39
         Top             =   2115
         Width           =   2445
      End
      Begin VB.Image Image4 
         Height          =   480
         Left            =   -74790
         Picture         =   "frmMain.frx":0910
         Top             =   1920
         Width           =   480
      End
      Begin VB.Label Label3 
         Caption         =   "M."
         Height          =   225
         Left            =   -69870
         TabIndex        =   36
         Top             =   1260
         Width           =   1170
      End
      Begin VB.Label Label2 
         Caption         =   "日志文件大小"
         Height          =   225
         Left            =   -71985
         TabIndex        =   34
         Top             =   1245
         Width           =   1170
      End
      Begin VB.Image Image3 
         Height          =   480
         Left            =   -74805
         Picture         =   "frmMain.frx":0D52
         Top             =   390
         Width           =   480
      End
      Begin VB.Label Label1 
         Caption         =   "公用设置"
         Height          =   210
         Left            =   -74130
         TabIndex        =   33
         Top             =   585
         Width           =   2445
      End
      Begin VB.Label lblLog 
         Caption         =   "查询结果"
         Height          =   210
         Index           =   6
         Left            =   -74145
         TabIndex        =   29
         Top             =   1650
         Width           =   2445
      End
      Begin VB.Image Image2 
         Height          =   480
         Left            =   -74820
         Picture         =   "frmMain.frx":1194
         Top             =   1455
         Width           =   480
      End
      Begin VB.Label lblLog 
         Caption         =   "截止日期/时间："
         Height          =   210
         Index           =   5
         Left            =   -70800
         TabIndex        =   25
         Top             =   1170
         Width           =   1455
      End
      Begin VB.Label lblLog 
         Caption         =   "开始日期/时间："
         Height          =   210
         Index           =   4
         Left            =   -74880
         TabIndex        =   24
         Top             =   1170
         Width           =   1455
      End
      Begin VB.Label lblLog 
         Caption         =   "查询条件设置"
         Height          =   210
         Index           =   3
         Left            =   -74160
         TabIndex        =   23
         Top             =   600
         Width           =   2445
      End
      Begin VB.Image Image1 
         Height          =   480
         Left            =   -74835
         Picture         =   "frmMain.frx":15D6
         Top             =   405
         Width           =   480
      End
      Begin VB.Label lblLog 
         Caption         =   "日志查询结果列表（共X页，当前为第X页，每页X笔）："
         Height          =   210
         Index           =   2
         Left            =   -74925
         TabIndex        =   22
         Top             =   2190
         Width           =   5295
      End
      Begin VB.Image imgLog 
         Height          =   480
         Left            =   180
         Picture         =   "frmMain.frx":1A18
         Top             =   360
         Width           =   480
      End
      Begin VB.Label lblLog 
         Caption         =   "封包监视"
         Height          =   210
         Index           =   0
         Left            =   855
         TabIndex        =   15
         Top             =   555
         Width           =   2445
      End
      Begin VB.Label lblApp 
         Caption         =   "应用程序设置"
         Height          =   210
         Left            =   -74130
         TabIndex        =   14
         Top             =   630
         Width           =   2445
      End
      Begin VB.Image imgApp 
         Height          =   480
         Left            =   -74805
         Picture         =   "frmMain.frx":1E5A
         Top             =   435
         Width           =   480
      End
      Begin VB.Label lblAcl 
         Caption         =   "控管规则设置"
         Height          =   210
         Index           =   0
         Left            =   -74130
         TabIndex        =   13
         Top             =   1815
         Width           =   2445
      End
      Begin VB.Image imgAcl 
         Height          =   480
         Left            =   -74805
         Picture         =   "frmMain.frx":229C
         Top             =   1620
         Width           =   480
      End
      Begin VB.Label lblAcl 
         Caption         =   "控管规则列表："
         Height          =   210
         Index           =   1
         Left            =   -74835
         TabIndex        =   12
         Top             =   2370
         Width           =   2445
      End
   End
   Begin VB.Label lblAbout 
      BackStyle       =   0  'Transparent
      Caption         =   "XSTUDIO"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000011&
      Height          =   180
      Index           =   0
      Left            =   75
      TabIndex        =   18
      Top             =   5565
      Width           =   1050
   End
   Begin VB.Label lblAbout 
      BackStyle       =   0  'Transparent
      Caption         =   "XSTUDIO"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   240
      Index           =   1
      Left            =   90
      TabIndex        =   19
      Top             =   5580
      Width           =   1200
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)
    Select Case Button.Key
        Case "add":
            frmAcl.Show 1
        Case "edit":
        Case "delete":
    End Select
End Sub
