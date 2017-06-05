VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TabCtl32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmInfo 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "网络与时间设置"
   ClientHeight    =   4785
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6960
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4785
   ScaleWidth      =   6960
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton cmdUsed 
      Caption         =   "应用(&A)"
      Height          =   300
      Left            =   5865
      TabIndex        =   3
      Top             =   4440
      Width           =   1050
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "取消"
      Height          =   300
      Left            =   4725
      TabIndex        =   2
      Top             =   4440
      Width           =   1050
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "确定"
      Default         =   -1  'True
      Height          =   300
      Left            =   3600
      TabIndex        =   1
      Top             =   4440
      Width           =   1050
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4335
      Left            =   45
      TabIndex        =   0
      Top             =   45
      Width           =   6885
      _ExtentX        =   12144
      _ExtentY        =   7646
      _Version        =   393216
      Style           =   1
      Tabs            =   2
      TabHeight       =   520
      TabCaption(0)   =   "网络设置"
      TabPicture(0)   =   "frmInfo.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label1"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Image1"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label4"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Toolbar1"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "TreeView1"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "ListView1"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Frame1"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).ControlCount=   7
      TabCaption(1)   =   "时间设置"
      TabPicture(1)   =   "frmInfo.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label2"
      Tab(1).Control(1)=   "Label3"
      Tab(1).Control(2)=   "Image2"
      Tab(1).Control(3)=   "Toolbar2"
      Tab(1).Control(4)=   "ListView2"
      Tab(1).Control(5)=   "TreeView2"
      Tab(1).Control(6)=   "Check1(0)"
      Tab(1).Control(7)=   "Check1(1)"
      Tab(1).Control(8)=   "Check1(2)"
      Tab(1).Control(9)=   "Check1(3)"
      Tab(1).Control(10)=   "Check1(4)"
      Tab(1).Control(11)=   "Check1(5)"
      Tab(1).Control(12)=   "Check1(6)"
      Tab(1).Control(13)=   "Frame2"
      Tab(1).Control(14)=   "Frame3"
      Tab(1).ControlCount=   15
      Begin VB.Frame Frame3 
         Height          =   60
         Left            =   -72750
         TabIndex        =   22
         Top             =   840
         Width           =   4560
      End
      Begin VB.Frame Frame1 
         Height          =   90
         Left            =   2265
         TabIndex        =   21
         Top             =   825
         Width           =   4560
      End
      Begin VB.Frame Frame2 
         Height          =   90
         Left            =   -72750
         TabIndex        =   20
         Top             =   1560
         Width           =   4560
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期六"
         Height          =   270
         Index           =   6
         Left            =   -70515
         TabIndex        =   19
         Top             =   1305
         Width           =   1050
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期五"
         Height          =   270
         Index           =   5
         Left            =   -71550
         TabIndex        =   18
         Top             =   1305
         Width           =   1050
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期四"
         Height          =   270
         Index           =   4
         Left            =   -72585
         TabIndex        =   17
         Top             =   1305
         Width           =   1050
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期三"
         Height          =   270
         Index           =   3
         Left            =   -69465
         TabIndex        =   16
         Top             =   945
         Width           =   1050
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期二"
         Height          =   270
         Index           =   2
         Left            =   -70515
         TabIndex        =   15
         Top             =   945
         Width           =   1050
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期一"
         Height          =   270
         Index           =   1
         Left            =   -71550
         TabIndex        =   14
         Top             =   945
         Width           =   1050
      End
      Begin VB.CheckBox Check1 
         Caption         =   "星期日"
         Height          =   270
         Index           =   0
         Left            =   -72585
         TabIndex        =   13
         Top             =   945
         Width           =   1050
      End
      Begin MSComctlLib.TreeView TreeView2 
         Height          =   3855
         Left            =   -74925
         TabIndex        =   8
         Top             =   375
         Width           =   2160
         _ExtentX        =   3810
         _ExtentY        =   6800
         _Version        =   393217
         Style           =   7
         Appearance      =   1
      End
      Begin MSComctlLib.ListView ListView1 
         Height          =   2955
         Left            =   2250
         TabIndex        =   5
         Top             =   1290
         Width           =   4575
         _ExtentX        =   8070
         _ExtentY        =   5212
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   0
      End
      Begin MSComctlLib.TreeView TreeView1 
         Height          =   3855
         Left            =   75
         TabIndex        =   4
         Top             =   375
         Width           =   2160
         _ExtentX        =   3810
         _ExtentY        =   6800
         _Version        =   393217
         Style           =   7
         Appearance      =   1
      End
      Begin MSComctlLib.Toolbar Toolbar1 
         Height          =   330
         Left            =   4245
         TabIndex        =   6
         Top             =   945
         Width           =   2565
         _ExtentX        =   4524
         _ExtentY        =   582
         ButtonWidth     =   1349
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
      Begin MSComctlLib.ListView ListView2 
         Height          =   2190
         Left            =   -72750
         TabIndex        =   9
         Top             =   2055
         Width           =   4575
         _ExtentX        =   8070
         _ExtentY        =   3863
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
         Left            =   -70770
         TabIndex        =   10
         Top             =   1710
         Width           =   2565
         _ExtentX        =   4524
         _ExtentY        =   582
         ButtonWidth     =   1349
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
      Begin VB.Label Label4 
         Caption         =   "网络信息："
         Height          =   180
         Left            =   2280
         TabIndex        =   23
         Top             =   1035
         Width           =   1380
      End
      Begin VB.Image Image2 
         Height          =   480
         Left            =   -72705
         Picture         =   "frmInfo.frx":0038
         Top             =   375
         Width           =   480
      End
      Begin VB.Image Image1 
         Height          =   480
         Left            =   2295
         Picture         =   "frmInfo.frx":047A
         Top             =   375
         Width           =   480
      End
      Begin VB.Label Label3 
         Caption         =   "星期与时间"
         Height          =   240
         Left            =   -72090
         TabIndex        =   12
         Top             =   570
         Width           =   1785
      End
      Begin VB.Label Label2 
         Caption         =   "时间："
         Height          =   255
         Left            =   -72705
         TabIndex        =   11
         Top             =   1785
         Width           =   1785
      End
      Begin VB.Label Label1 
         Caption         =   "网络信息"
         Height          =   180
         Left            =   2955
         TabIndex        =   7
         Top             =   555
         Width           =   1380
      End
   End
End
Attribute VB_Name = "frmInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
