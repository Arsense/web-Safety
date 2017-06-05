VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "Comdlg32.OCX"
Begin VB.Form frmAcl 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "应用程序规则"
   ClientHeight    =   6615
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6240
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6615
   ScaleWidth      =   6240
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin MSComDlg.CommonDialog cdApp 
      Left            =   195
      Top             =   4485
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdUsed 
      Caption         =   "应用(&A)"
      Height          =   300
      Left            =   5040
      TabIndex        =   27
      Top             =   6210
      Width           =   1050
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "取消"
      Height          =   300
      Left            =   3900
      TabIndex        =   26
      Top             =   6210
      Width           =   1050
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "确定"
      Default         =   -1  'True
      Height          =   300
      Left            =   2775
      TabIndex        =   25
      Top             =   6210
      Width           =   1050
   End
   Begin VB.Frame Frame1 
      Height          =   90
      Left            =   -15
      TabIndex        =   24
      Top             =   5985
      Width           =   6255
   End
   Begin VB.TextBox Text2 
      Height          =   1485
      Left            =   1470
      TabIndex        =   23
      Text            =   "Text2"
      Top             =   4440
      Width           =   4605
   End
   Begin VB.TextBox Text1 
      Height          =   300
      Left            =   1470
      TabIndex        =   19
      Text            =   "Text1"
      Top             =   3525
      Width           =   3600
   End
   Begin VB.Frame Frame7 
      Height          =   90
      Left            =   15
      TabIndex        =   13
      Top             =   4230
      Width           =   6255
   End
   Begin VB.ComboBox Combo4 
      Height          =   300
      Left            =   1470
      TabIndex        =   12
      Text            =   "Combo1"
      Top             =   3150
      Width           =   3600
   End
   Begin VB.ComboBox Combo3 
      Height          =   300
      Left            =   1470
      TabIndex        =   11
      Text            =   "Combo1"
      Top             =   2775
      Width           =   3600
   End
   Begin VB.Frame Frame5 
      Height          =   90
      Left            =   -30
      TabIndex        =   9
      Top             =   2595
      Width           =   6255
   End
   Begin VB.ComboBox Combo2 
      Height          =   300
      Left            =   1470
      TabIndex        =   8
      Text            =   "Combo1"
      Top             =   1695
      Width           =   3600
   End
   Begin VB.CommandButton cmdTime 
      Caption         =   "..."
      Height          =   285
      Left            =   5220
      TabIndex        =   7
      Top             =   1350
      Width           =   420
   End
   Begin VB.ComboBox Combo1 
      Height          =   300
      Left            =   1470
      TabIndex        =   6
      Text            =   "Combo1"
      Top             =   1335
      Width           =   3600
   End
   Begin VB.Frame Frame2 
      Height          =   90
      Left            =   0
      TabIndex        =   4
      Top             =   435
      Width           =   6255
   End
   Begin VB.ComboBox cbNet 
      Height          =   300
      Left            =   1470
      TabIndex        =   3
      Text            =   "Combo1"
      Top             =   975
      Width           =   3600
   End
   Begin VB.CommandButton cmdNet 
      Caption         =   "..."
      Height          =   285
      Left            =   5220
      TabIndex        =   2
      Top             =   990
      Width           =   420
   End
   Begin VB.ComboBox cbApp 
      Height          =   300
      Left            =   1470
      TabIndex        =   1
      Text            =   "Combo1"
      Top             =   615
      Width           =   3600
   End
   Begin VB.CommandButton cmdApp 
      Caption         =   "..."
      Height          =   285
      Left            =   5220
      TabIndex        =   0
      Top             =   630
      Width           =   420
   End
   Begin VB.Label Label1 
      Caption         =   "服务端口："
      Height          =   285
      Index           =   6
      Left            =   390
      TabIndex        =   22
      Top             =   3540
      Width           =   1020
   End
   Begin VB.Label Label1 
      Caption         =   "服务类型："
      Height          =   285
      Index           =   5
      Left            =   390
      TabIndex        =   21
      Top             =   3165
      Width           =   1020
   End
   Begin VB.Label Label1 
      Caption         =   "进出方向："
      Height          =   285
      Index           =   4
      Left            =   390
      TabIndex        =   20
      Top             =   2805
      Width           =   1020
   End
   Begin VB.Label lblLog 
      Caption         =   "备注"
      Height          =   210
      Index           =   6
      Left            =   825
      TabIndex        =   18
      Top             =   3990
      Width           =   2445
   End
   Begin VB.Label Label1 
      Caption         =   "管制动作："
      Height          =   285
      Index           =   3
      Left            =   390
      TabIndex        =   17
      Top             =   1725
      Width           =   1020
   End
   Begin VB.Label Label1 
      Caption         =   "访问时间："
      Height          =   285
      Index           =   2
      Left            =   390
      TabIndex        =   16
      Top             =   1350
      Width           =   1020
   End
   Begin VB.Label Label1 
      Caption         =   "远端网络："
      Height          =   285
      Index           =   1
      Left            =   390
      TabIndex        =   15
      Top             =   975
      Width           =   1020
   End
   Begin VB.Label Label1 
      Caption         =   "应用程序："
      Height          =   285
      Index           =   0
      Left            =   390
      TabIndex        =   14
      Top             =   630
      Width           =   1020
   End
   Begin VB.Image Image7 
      Height          =   480
      Left            =   150
      Picture         =   "frmAcl.frx":0000
      Top             =   3795
      Width           =   480
   End
   Begin VB.Image Image5 
      Height          =   480
      Left            =   105
      Picture         =   "frmAcl.frx":0442
      Top             =   2160
      Width           =   480
   End
   Begin VB.Label lblLog 
      Caption         =   "高级设置"
      Height          =   210
      Index           =   4
      Left            =   780
      TabIndex        =   10
      Top             =   2355
      Width           =   2445
   End
   Begin VB.Image Image1 
      Height          =   480
      Left            =   135
      Picture         =   "frmAcl.frx":0884
      Top             =   0
      Width           =   480
   End
   Begin VB.Label lblLog 
      Caption         =   "基本设置"
      Height          =   210
      Index           =   3
      Left            =   810
      TabIndex        =   5
      Top             =   195
      Width           =   2445
   End
End
Attribute VB_Name = "frmAcl"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdApp_Click()
    cdApp.ShowOpen
End Sub

Private Sub cmdNet_Click()
    frmInfo.SSTab1.Tab = 0
    frmInfo.Show 1
End Sub

Private Sub cmdTime_Click()
    frmInfo.SSTab1.Tab = 1
    frmInfo.Show 1
End Sub
