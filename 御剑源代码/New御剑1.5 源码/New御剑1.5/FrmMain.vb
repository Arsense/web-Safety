Public Class FrmMain
    Dim My_IP, My_Flag As String '旁注或C段当前IP,停止操作标记
    Dim ProgressBar_Index, ProgressBar_Max, ProgressBar_Time As Int32 '绑定域名查询进度、总量、耗时

    Dim Scan_Index, Scan_Max, Scan_JD, Scan_TJ, Scan_SL As Int32 '批量扫描索引、总量、进度、速度、数量
    Dim Scan_Flag As Int32 '扫描操作标志，1扫描，0停止，3继续，2暂停
    Dim Scan_list As New List(Of String) '扫描字典集合
    Dim Scan_Mode As String '扫描模式
    Dim Scan_TimeOut As Int32 '超时
    Dim Scan_TB As New Object '扫描同步对象

    Dim SQL_i As Int32 'SQL采集域名索引
    Dim SQL_JD As Int32 '采集进度
    Dim SQL_TB As New Object 'SQL同步对象
    Dim SQL_Sort As New List(Of String) '过滤重复注入点
    Dim Sort_KG As Boolean '排序
    Dim SQL_Exit As Boolean '推出
    Dim SQL_Task As Int32 '线程

    Dim CD As String '全局菜单

    Dim MD5_Index, MD5_JD, MD5_Task As Int32 '线程索引、进度、剩余线程
    Dim MD5_TB As New Object
    Dim MD5_I As Int32

    Dim MYtxtIP As String

    Private Sub Frm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ' 1 - 窗体初始化
        Me.Show()
        Control.CheckForIllegalCrossThreadCalls = False
        If Not (IO.Directory.Exists(Application.StartupPath & "\御剑配置文件\")) Then MkDir(Application.StartupPath & "\御剑配置文件\")
        If Not (IO.Directory.Exists(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\")) Then MkDir(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\")
        Dim GO As New Threading.Thread(AddressOf Fuck_Title)
        GO.Start()
        TreeView1.Sort()
    End Sub

    Private Sub FrmMain_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        Dim FileNum As Int32 = FreeFile()
        FileOpen(FileNum, Application.StartupPath & "\御剑配置文件\Set.ini", OpenMode.Output)
        Print(FileNum, ComboBox3.Text & "," & ComboBox2.Text & "," & ComboBox1.Text & "," & ComboBox4.Text & "," & ComboBox5.Text & "," & TextBox2.Text & "," & TextBox3.Text)
        FileClose(FileNum)
        ' 2 - 强制退出程序
        Environment.Exit(0)
    End Sub

    Private Sub Fuck_Title()
        On Error Resume Next
        Dim txt As String = "TUM5dE9XOTZSWFZPWVVjeWVpdDJSVGR5VURGM1lsZG9kSGxDUTFkaFR6WXdMMjA1YnpkdVEzUnpRV2RWVmtVMlRYcFJlazFFVFRCT2FsVXk="
        Dim temp As String = ""
        Dim str() As Byte
        For x As Int32 = 1 To 3
            str = System.Convert.FromBase64String(txt)
            temp = System.Text.Encoding.Default.GetString(str)
            txt = temp
        Next
        Me.Text = temp
        Me.Text = Me.Text.Insert(5, "内部版")
        Me.Text = Me.Text.Insert(Me.Text.Length, Space(4) & "专属：" & Label35.Text)

        ListView1.GridLines = True
        ListView1.FullRowSelect = True
        ListView1.View = View.Details
        ListView1.Columns.Add(Space(1) & "ID", 40)
        ListView1.Columns.Add(Space(35) & "地址", 470)
        ListView1.Columns.Add("HTTP响应", 75, HorizontalAlignment.Center)

        ListView2.GridLines = True
        ListView2.FullRowSelect = True
        ListView2.View = View.Details
        ListView2.Columns.Add(Space(1) & "ID", 40)
        ListView2.Columns.Add(Space(16) & "可能存在注入的连接地址(线程过大容易导致超时和丢包)", 545)

        For x As Int32 = 3 To 30
            ComboBox1.Items.Add(x.ToString)
        Next
        ComboBox1.Text = "3"
        If Environment.ProcessorCount * 10 > 100 Then
            For x As Int32 = 1 To Environment.ProcessorCount * 10
                ComboBox2.Items.Add(x.ToString)
            Next
            ComboBox2.Text = Environment.ProcessorCount * 5
        Else
            For x As Int32 = 1 To 100
                ComboBox2.Items.Add(x.ToString)
                ComboBox4.Items.Add(x.ToString)
            Next
            For x As Int32 = 1 To 40
                ComboBox5.Items.Add(x.ToString)
            Next
            ComboBox2.Text = Environment.ProcessorCount * 10
        End If
        If Not (IO.Directory.Exists(Application.StartupPath & "\御剑配置文件\")) Then MkDir(Application.StartupPath & "\御剑配置文件\")
        ComboBox3.Items.Add("HEAD - 速度极快")
        ComboBox3.Items.Add("GET - 标准请求")
        ComboBox3.Text = "HEAD - 速度极快"
        If Dir(Application.StartupPath & "\御剑配置文件\Set.ini") <> "" Then
            temp = My.Computer.FileSystem.ReadAllText(Application.StartupPath & "\御剑配置文件\Set.ini", System.Text.Encoding.Default)
            Dim list() As String = temp.Split(",")
            ComboBox3.Text = list(0)
            ComboBox2.Text = list(1)
            ComboBox1.Text = list(2)
            ComboBox4.Text = list(3)
            ComboBox5.Text = list(4)
            TextBox2.Text = list(5)
            TextBox3.Text = list(6)
        End If
        Dim di As IO.DirectoryInfo = New IO.DirectoryInfo(Application.StartupPath & "\御剑配置文件\")
        Dim fii() As IO.FileInfo = di.GetFiles("*.txt", IO.SearchOption.TopDirectoryOnly)
        For x As Int32 = 0 To UBound(fii)
            ListBox2.Items.Add(fii(x).ToString & "-可用")
        Next

        Dim client As Net.WebClient = New Net.WebClient()
        Dim bytRecv As Byte() = client.DownloadData("http://www.ip138.com/ip2city.asp")
        Dim str1 As String = System.Text.Encoding.GetEncoding("gb2312").GetString(bytRecv)
        Dim r As String = "(((\d{1,3})|(1\d{2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,3})|(1\d{2})|(2[0-4]\d)|(25[0-5]))"
        Dim ip As String = System.Text.RegularExpressions.Regex.Match(str1, r).ToString()
        MYtxtIP = (Net.IPAddress.Parse(ip).ToString)


    End Sub

    Private Function MidUrl(ByVal URL As String) As String  '截断域名
        On Error Resume Next
        URL = URL.ToLower
        URL = Replace(URL, "http://", "")
        URL = Replace(URL, "https://", "")
        Dim X As Int32 = URL.IndexOf("/")
        If X > 0 Then URL = URL.Substring(0, X)
        Return URL
    End Function

    Private Function GetIP(ByVal URL As String) As String  '获取IP
        Try
            Label4.Text = "指令：正在解析域名..."
            ListBox_IP.Items.Clear()
            My_IP = ""
            Dim IPS() As Net.IPAddress
            IPS = Net.Dns.GetHostAddresses(URL)
            If IPS.Length = 1 Then
                My_IP = IPS(0).ToString
                Dim x As Int32 = My_IP.LastIndexOf(".") + 1
                Dim tempIP As String = My_IP
                Dim temp As String = tempIP.Substring(x, tempIP.Length - x)
                If RadioButton2.Checked Then tempIP = tempIP.Replace(temp, "1")
                TextBox_StartIP.Text = tempIP
                If CDN(URL) Then
                    ListBox_IP.Items.Add(IPS(0).ToString & " - CDN逆向IP【Del键移除选中项】")
                Else
                    ListBox_IP.Items.Add("该域名不存在CDN服务器")
                End If
            Else
                If IPS.Length > 0 Then
                    TextBox_StartIP.Text = ""
                    TextBox_EndIP.Text = ""
                    For x As Int32 = 0 To IPS.Length - 1
                        ListBox_IP.Items.Add(IPS(x).ToString & " - CDN逆向IP【Del键移除选中项】")
                    Next
                Else
                    ListBox_IP.Items.Add("无法解析域名")
                End If
            End If
            Label4.Text = "指令：域名解析成功..."
        Catch ex As Exception
            Label4.Text = "指令：域名解析失败..."
        End Try
    End Function

    Private Function CDN(ByVal url As String) As Boolean
        Dim My_Ping As New Process
        My_Ping.StartInfo.RedirectStandardOutput = True '使用进程输出 
        My_Ping.StartInfo.CreateNoWindow = True '新窗口来响应进程 
        My_Ping.StartInfo.FileName = "ping"
        My_Ping.StartInfo.Arguments = " -n 1 -w 100 " & url
        My_Ping.StartInfo.UseShellExecute = False '这里一定要设置为FALSE 
        My_Ping.Start()
        Dim txt As String = My_Ping.StandardOutput.ReadToEnd
        If txt.IndexOf(url) > 0 Then
            Return False
        Else
            Return True
        End If
    End Function

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        If TextBox_Url.Text.Length > 0 Then
            Dim G As New Threading.Thread(AddressOf GET_IP_SUB)
            G.Start()
        End If
    End Sub

    Private Sub GET_IP_SUB()
        TextBox_Url.Text = MidUrl(TextBox_Url.Text)
        GetIP(TextBox_Url.Text)
    End Sub

    Private Sub TextBox_StartIP_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TextBox_StartIP.TextChanged
        Try
            If My_IP.Length < 3 Then Exit Try
            Dim x1 As Int32 = My_IP.LastIndexOfAny(".") + 1
            Dim temp As String = My_IP.Substring(0, x1)
            TextBox_EndIP.Text = temp & "254"
        Catch ex As Exception
        End Try
    End Sub

    Private Sub RadioButton3_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton3.CheckedChanged
        TextBox_StartIP.Text = My_IP
        TextBox_StartIP.Enabled = Not TextBox_StartIP.Enabled
        TextBox_EndIP.Enabled = Not TextBox_EndIP.Enabled
    End Sub

    Private Sub RadioButton2_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton2.CheckedChanged
        Try
            If My_IP.Length < 3 Then Exit Try
            Dim x1 As Int32 = My_IP.LastIndexOfAny(".") + 1
            Dim temp As String = My_IP.Substring(0, x1)
            TextBox_StartIP.Text = temp & "1"
            TextBox_EndIP.Text = temp & "254"
        Catch ex As Exception
        End Try
    End Sub

    Private Sub RadioButton1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton1.CheckedChanged
        TextBox_StartIP.Text = My_IP
    End Sub

    Private Sub TextBox_Url_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TextBox_Url.TextChanged
        TextBox_StartIP.Text = ""
        TextBox_EndIP.Text = ""
        If ListBox_IP.Items.Count > 0 Then ListBox_IP.Items.Clear()
    End Sub

    Private Sub ListBox_IP_KeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles ListBox_IP.KeyDown
        If e.KeyCode = 8 Or e.KeyCode = 46 Or e.KeyCode = 110 Then
            Try
                For x As Int32 = ListBox_IP.Items.Count - 1 To 0 Step -1
                    ListBox_IP.Items.RemoveAt(ListBox_IP.SelectedIndex)
                Next
            Catch ex As Exception
            End Try
        End If
    End Sub

    Private Sub ListBox_IP_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ListBox_IP.SelectedIndexChanged
        Try
            If RadioButton2.Checked Then
                My_IP = ListBox_IP.SelectedItem.ToString
                My_IP = My_IP.Replace(" - CDN逆向IP【Del键移除选中项】", "")
                RadioButton2_CheckedChanged(sender, e)
            End If
            If RadioButton3.Checked Then
                My_IP = ListBox_IP.SelectedItem.ToString
                My_IP = My_IP.Replace(" - CDN逆向IP【Del键移除选中项】", "")
                TextBox_StartIP.Text = My_IP
            End If
        Catch ex As Exception
        End Try
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        On Error Resume Next
        If TextBox_StartIP.Text.Length = ListBox_IP.Items.Count Then
            MsgBox("请先获取IP地址！", , "提示")
            Exit Sub
        End If
        ListBox_Command.Items.Clear()
        TreeView1.Nodes.Clear()
        ProgressBar_Index = 0
        Dim temp As String
        If RadioButton1.Checked Then
            If ListBox_IP.Items.Item(0) <> "该域名不存在CDN服务器" Then
                For x As Int32 = 0 To ListBox_IP.Items.Count - 1
                    temp = ListBox_IP.Items.Item(x)
                    temp = temp.Replace(" - CDN逆向IP【Del键移除选中项】", "")
                    ListBox_Command.Items.Add(temp)
                Next
            Else
                ListBox_Command.Items.Add(TextBox_StartIP.Text)
            End If
        Else
            If TextBox_StartIP.Text.Length = 0 Then
                MsgBox("请先在右边CDN逆向列表指定一个开始IP", , "提示")
                Exit Sub
            End If
            Dim C_IP As String
            Dim C_Min, C_Max, index As Int32
            index = TextBox_StartIP.Text.LastIndexOfAny(".") + 1
            C_IP = TextBox_StartIP.Text.Substring(0, index)
            C_Min = TextBox_StartIP.Text.Substring(index, TextBox_StartIP.Text.Length - index)
            C_Max = TextBox_EndIP.Text.Substring(index, TextBox_EndIP.Text.Length - index)
            If C_Max < C_Min Then
                MsgBox("结束IP不能小于开始IP", , "提示")
                Exit Sub
            End If
            For x As Int32 = C_Min To C_Max
                ListBox_Command.Items.Add(C_IP & x.ToString)
            Next
        End If
        Button2.Enabled = False
        Kill(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\*.*")
        ProgressBar_Max = ListBox_Command.Items.Count
        ProgressBar_C.Minimum = 0
        ProgressBar_C.Maximum = ProgressBar_Max
        ProgressBar_C.Value = 0
        ProgressBar_Time = Environment.TickCount
        Label4.Text = "指令：正在查询绑定域名..."
        Label5.Text = "耗时：统计中..."
        Label6.Text = "数据：统计中..."
        Label7.Text = "节点：统计中..."
        My_Flag = "OK"
        For x As Int32 = 0 To ListBox_Command.Items.Count - 1
            temp = ListBox_Command.Items.Item(x)
            Dim task As New TaskThread(temp)
            Dim GO As New Threading.Thread(AddressOf Query_SUB)
            GO.IsBackground = True
            GO.Start(task)
        Next
        Timer_C.Start()
        Button3.Enabled = True
    End Sub

    Private Sub Query_SUB(ByVal data As Object)
        On Error Resume Next
        Dim Task As TaskThread = data
        Dim i, i1, i2, max_i As Int32
        Dim temp, newtemp As String
        i = 0
        Do
            If My_Flag = "NO" Then Exit Do
            temp = GetHtmlBody("http://cn.bing.com/search?first=" & i & "&count=50&q=ip:" & Task.IP, "UTF-8")
            i1 = temp.IndexOf("<h3>")
            i2 = temp.LastIndexOf("</h3>")
            newtemp &= temp
            i += 50
            If i > 150 Then Exit Do
        Loop While temp.IndexOf("下一页") > -1
        C_MidSub(newtemp, Task.IP)
        If My.Computer.FileSystem.FileExists(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\" & Task.IP) Then
            Rename(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\" & Task.IP, Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\" & Task.IP & ".txt")
        End If
        Threading.Interlocked.Increment(ProgressBar_Index)
        If CheckBox2.Checked Then
            Label4.Text = "指令：验证IP[" & Task.IP & "]"
        Else
            Label4.Text = "指令：正在查询绑定域名[ " & ProgressBar_Index & "/" & ProgressBar_Max & " ]"
        End If
    End Sub

    Private Function GetHtmlBody(ByVal URL As String, ByVal Code As String) As String '获取HTML源码
        On Error Resume Next
        Dim Request As System.Net.HttpWebRequest
        Dim Response As System.Net.HttpWebResponse
        Dim httpURL As New System.Uri(URL)
        Request = CType(Net.WebRequest.Create(httpURL), Net.HttpWebRequest)
        Request.Method = "GET"
        Request.Timeout = 30 * 1000
        Request.AllowAutoRedirect = False
        Request.AllowWriteStreamBuffering = True
        Request.ReadWriteTimeout = 30 * 1000
        Request.ServicePoint.ConnectionLimit = 512
        Response = CType(Request.GetResponse(), Net.HttpWebResponse)
        Dim Encoding As String = Response.CharacterSet.ToUpper
        If InStr(Encoding, "UTF-8") = 0 Then Encoding = "GB2312"
        Dim reader As New IO.StreamReader(Response.GetResponseStream, System.Text.Encoding.GetEncoding(Encoding))
        Dim temp As String = reader.ReadToEnd
        Return temp
        Request.Abort()
        reader.Close()
        Response.Close()
    End Function

    Private Sub C_MidSub(ByVal temp As String, ByVal IPS As String)
        '切割C段列表过程
        On Error Resume Next
        Dim c_Url As New System.Text.StringBuilder
        Dim i1, i2, i3, i4 As Int32
        Dim txt, txtip, url, title As String
        Dim FileNum As Int32 = FreeFile()
        FileOpen(FileNum, Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\" & IPS, OpenMode.Append)
        For x As Int32 = 0 To temp.Length - 1
            i3 = 0
            i4 = 0
            If My_Flag = "NO" Then Exit For
            i1 = temp.IndexOf("<h3>", i1) + 4
            i2 = temp.IndexOf("</h3>", i1)
            If i1 < x Then Exit For
            txt = temp.Substring(i1, i2 - i1)
            i3 = txt.IndexOf("""", i3) + 1
            i4 = txt.IndexOf("""", i3)
            url = txt.Substring(i3, i4 - i3)
            i3 = txt.IndexOf(">", i3) + 1
            i4 = txt.IndexOf("<", i3)
            title = txt.Substring(i3, i4 - i3)
            url = MidUrl(url)
            If InStr(c_Url.ToString, url) = 0 Then
                If CheckBox2.Checked Then
                    If GetMyIP(url) = IPS Then
                        c_Url.Append(url)
                        Print(FileNum, "http://" & url & " - " & title & "剑")
                    End If
                Else
                    c_Url.Append(url)
                    Print(FileNum, "http://" & url & " - " & title & "剑")
                End If
            End If
            i1 = i2
        Next
        FileClose(FileNum)
    End Sub

    Private Sub Timer_C_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer_C.Tick
        On Error Resume Next
        ProgressBar_C.Value = ProgressBar_Index
        ADD_C_LIST()
        If ProgressBar_Index = ProgressBar_Max Then
            ADD_C_LIST()
            Dim l, ll As Int32
            For x As Int32 = TreeView1.Nodes.Count - 1 To 0 Step -1
                l = TreeView1.Nodes(x).Nodes.Count
                If l = 0 Then
                    TreeView1.Nodes(x).Remove()
                Else
                    ll += l
                End If
            Next
            Label4.Text = "指令：正在探测服务器信息..."
            Label5.Text = "耗时：" & (Environment.TickCount - ProgressBar_Time) \ 1000 & "秒"
            Label6.Text = "数据：" & ll & "条"
            Label7.Text = "节点：" & TreeView1.Nodes.Count & "个"
            If CheckBox1.Checked Then
                For x As Int32 = 0 To TreeView1.Nodes.Count - 1
                    Dim task As New TaskThread(x.ToString)
                    Dim GO As New Threading.Thread(AddressOf GetService_SUB)
                    GO.IsBackground = True
                    GO.Start(task)
                Next
            End If
            Button2.Enabled = True
            Button3.Enabled = False
            Label4.Text = "指令：查询完成..."
            Timer_C.Stop()
        End If
    End Sub

    Private Sub ADD_C_LIST()
        On Error Resume Next
        Dim di As IO.DirectoryInfo = New IO.DirectoryInfo(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\")
        Dim fii() As IO.FileInfo = di.GetFiles("*.txt", IO.SearchOption.AllDirectories)
        Dim list(), ip As String
        Dim temp As New System.Text.StringBuilder
        Dim i1 As Int32
        Dim l, ll As Int32
        Dim gl As New List(Of String)
        If UBound(fii) < 0 Then Exit Sub
        For x As Int32 = 0 To UBound(fii)
            If My_Flag = "NO" Then Exit For
            temp.Append(My.Computer.FileSystem.ReadAllText(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\" & fii(x).ToString, System.Text.Encoding.Default))
            My.Computer.FileSystem.DeleteFile(Application.StartupPath & "\御剑配置文件\高速缓冲文件夹\" & fii(x).ToString)
            list = temp.ToString.Split("剑")
            temp.Remove(0, temp.Length)
            i1 = fii(x).ToString.LastIndexOf(".")
            ip = fii(x).ToString.Substring(0, i1)
            Dim node As New TreeNode
            gl.Clear()
            For xx As Int32 = 0 To UBound(list)
                If list(xx).Length > 11 Then
                    If Not gl.Contains(list(xx)) Then
                        node.Nodes.Add(list(xx))
                        gl.Add(list(xx))
                    End If
                End If
            Next
            ll += l
            node.Text = ip & " - [" & node.Nodes.Count & "]"
            If node.Nodes.Count > 0 Then
                TreeView1.Nodes.Add(node)
            End If
        Next
    End Sub

    Private Sub GetService_SUB(ByVal data As Object)
        On Error Resume Next
        Application.DoEvents()
        Dim Task As TaskThread = data
        Dim i As Int32 = Val(Task.IP)
        Dim temp As String = TreeView1.Nodes(i).Nodes(0).Text
        Dim x As Int32 = temp.IndexOf(" ")
        temp = temp.Substring(0, x)
        Dim str As String = Get_HttpService(temp)
        TreeView1.Nodes(i).Text &= " - " & str
    End Sub

    Private Function Get_HttpService(ByVal url As String)
        Try
            Dim MyRequest2 As System.Net.HttpWebRequest
            Dim MyResponse2 As System.Net.HttpWebResponse
            MyRequest2 = CType(Net.WebRequest.Create(url), Net.HttpWebRequest)
            MyRequest2.Method = "HEAD"
            MyRequest2.ServicePoint.ConnectionLimit = TreeView1.Nodes.Count * 2
            MyRequest2.Proxy = Nothing
            MyRequest2.AllowAutoRedirect = False
            MyRequest2.Timeout = 6000
            MyRequest2.ReadWriteTimeout = 6000
            MyResponse2 = CType(MyRequest2.GetResponse(), Net.HttpWebResponse)
            MyRequest2.Abort()
            Return MyResponse2.Server
            MyResponse2.Close()
        Catch ex As Exception
            Return "超时"
        End Try
    End Function

    Private Function GetMyIP(ByVal URL As String) As String  '获取IP
        Try
            Dim IPS() As Net.IPAddress
            IPS = Net.Dns.GetHostAddresses(URL)
            If IPS.Length > 0 Then
                Return IPS(0).ToString
            Else
                Return "无法解析域名"
            End If
        Catch ex As Exception
            Return "无法解析域名"
        End Try
    End Function

    Private Sub TreeView1_MouseClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles TreeView1.MouseClick
        Dim node As TreeNode = TreeView1.GetNodeAt(e.Location)
        If node IsNot Nothing Then TreeView1.SelectedNode = node
    End Sub

    Private Sub TreeView1_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles TreeView1.MouseDoubleClick
        Try
            If InStr(TreeView1.SelectedNode.Text, "http") > 0 Then
                Dim i As Int32
                i = InStr(TreeView1.SelectedNode.Text, " ")
                Dim url As String = Mid(TreeView1.SelectedNode.Text, 1, i - 1)
                Process.Start(url)
            End If
        Catch ex As Exception
        End Try
    End Sub

    Private Sub TreeView1_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles TreeView1.MouseUp
        If e.Button = Windows.Forms.MouseButtons.Right And TreeView1.Nodes.Count > 0 Then
            ContextMenuStrip1.Show(Control.MousePosition.X, Control.MousePosition.Y)
        End If
    End Sub

    Private Sub 打开网址ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 打开网址ToolStripMenuItem.Click
        If InStr(TreeView1.SelectedNode.Text, "http") > 0 Then
            Dim i As Int32
            i = InStr(TreeView1.SelectedNode.Text, " ")
            Dim url As String = Mid(TreeView1.SelectedNode.Text, 1, i - 1)
            Process.Start(url)
        End If
    End Sub

    Private Sub 复制网址ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 复制网址ToolStripMenuItem.Click
        If InStr(TreeView1.SelectedNode.Text, "http") > 0 Then
            Dim i As Int32
            i = InStr(TreeView1.SelectedNode.Text, " ")
            Dim url As String = Mid(TreeView1.SelectedNode.Text, 1, i - 1)
            My.Computer.Clipboard.SetText(url)
        End If
    End Sub

    Private Sub 复制标题ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 复制标题ToolStripMenuItem.Click
        If InStr(TreeView1.SelectedNode.Text, "http") > 0 Then
            Dim i As Int32
            i = InStr(TreeView1.SelectedNode.Text, " - ") + 3
            Dim url As String = Mid(TreeView1.SelectedNode.Text, i, Len(TreeView1.SelectedNode.Text) - i)
            My.Computer.Clipboard.SetText(url)
        End If
    End Sub


    Private Sub 展开节点ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 展开节点ToolStripMenuItem.Click
        If TreeView1.Nodes.Count > 0 Then
            TreeView1.ExpandAll()
        End If
    End Sub

    Private Sub 折叠节点ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 折叠节点ToolStripMenuItem.Click
        If TreeView1.Nodes.Count > 0 Then
            TreeView1.CollapseAll()
        End If
    End Sub

    Private Sub 删除节点ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 删除节点ToolStripMenuItem.Click
        TreeView1.SelectedNode.Remove()
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click, Button16.Click
        Try
            If sender.name = "Button5" Then
                For x As Int32 = ListBox1.Items.Count - 1 To 0 Step -1
                    ListBox1.Items.RemoveAt(ListBox1.SelectedIndex)
                Next
            Else
                For x As Int32 = ListBox4.Items.Count - 1 To 0 Step -1
                    ListBox4.Items.RemoveAt(ListBox4.SelectedIndex)
                Next
            End If
        Catch ex As Exception
        End Try
        If sender.name = "Button5" Then
            Label8.Text = "作业数量：" & ListBox1.Items.Count
        Else
            Label14.Text = "作业数量：" & ListBox4.Items.Count
        End If
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click, Button17.Click
        On Error Resume Next
        Dim txt As String = InputBox("请输入一个新的网址：", "添加新网址")
        If txt.Length > 0 Then
            txt = txt.ToLower
            Dim https As Boolean = False
            If InStr(txt, "https") > 0 Then https = True
            txt = txt.Replace("https://", "")
            txt = txt.Replace("http://", "")
            Dim x1, x2 As Int32
            x1 = txt.LastIndexOf(".")
            x2 = txt.LastIndexOf("/")
            If x2 > x1 Then
                txt = txt & "/"
                txt = txt.Replace("//", "/")
                x1 = txt.LastIndexOf("/")
                txt = txt.Substring(0, x1)
            Else
                If x2 > 0 Then txt = txt.Substring(0, x2)
            End If
            If https Then
                If sender.name = "Button4" Then
                    ListBox1.Items.Add("https://" & txt)
                Else
                    ListBox4.Items.Add("https://" & txt)
                End If
            Else
                If sender.name = "Button4" Then
                    ListBox1.Items.Add("http://" & txt)
                Else
                    ListBox4.Items.Add("http://" & txt)
                End If
            End If
        End If
        If sender.name = "Button4" Then
            Label8.Text = "作业数量：" & ListBox1.Items.Count
        Else
            Label14.Text = "作业数量：" & ListBox4.Items.Count
        End If
    End Sub

    Private Sub ListBox2_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListBox2.MouseDoubleClick
        Try
            Dim temp As String = ListBox2.SelectedItem
            temp = temp.Replace("可用", "使用")
            ListBox3.Items.Add(temp)
            ListBox2.Items.Remove(ListBox2.SelectedItem)
        Catch ex As Exception
        End Try
    End Sub

    Private Sub ListBox3_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListBox3.MouseDoubleClick
        Try
            Dim temp As String = ListBox3.SelectedItem
            temp = temp.Replace("使用", "可用")
            ListBox2.Items.Add(temp)
            ListBox3.Items.Remove(ListBox3.SelectedItem)
        Catch ex As Exception
        End Try
    End Sub

    Private Sub ListBox_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListBox1.MouseDoubleClick, ListBox4.MouseDoubleClick, ListBox5.MouseDoubleClick
        Try
            Dim url As String = ""
            Select Case sender.name
                Case "ListBox1"
                    url = ListBox1.SelectedItem
                Case "ListBox4"
                    url = ListBox4.SelectedItem
                Case "ListBox5"
                    url = ListBox5.SelectedItem
            End Select
            If url.Length > 3 Then Process.Start(url)
        Catch ex As Exception
        End Try
    End Sub

    Private Sub ListBox_MouseLeave(ByVal sender As Object, ByVal e As System.EventArgs) Handles ListBox2.MouseLeave, ListBox3.MouseLeave
        sender.SelectedIndex = -1
    End Sub

    Private Sub XiQu_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button10.Click, Button15.Click
        On Error Resume Next
        If sender.name = "Button10" Then
            ListBox1.Items.Clear()
        Else
            ListBox4.Items.Clear()
        End If
        If TreeView1.Nodes.Count > 0 Then
            Dim url As String
            Dim i As Int32
            For x As Int32 = 0 To TreeView1.Nodes.Count - 1
                For xx As Int32 = 0 To TreeView1.Nodes(x).Nodes.Count - 1
                    url = TreeView1.Nodes(x).Nodes(xx).Text
                    i = url.IndexOf(" ")
                    If i > 0 Then
                        If sender.name = "Button10" Then
                            ListBox1.Items.Add(url.Substring(0, i))
                        Else
                            ListBox4.Items.Add(url.Substring(0, i))
                        End If
                    End If
                Next
            Next
            If sender.name = "Button10" Then
                Label8.Text = "作业数量：" & ListBox1.Items.Count
            Else
                Label14.Text = "作业数量：" & ListBox4.Items.Count
            End If
        Else
            MsgBox("域名列表为空", , "提示")
        End If
    End Sub

    Private Sub Button11_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button11.Click, Button14.Click
        On Error Resume Next
        If sender.name = "Button11" Then
            ListBox1.Items.Clear()
        Else
            ListBox4.Items.Clear()
        End If
        OpenFileDialog1.Filter = "所有文件 (*.*)|*.*|txt文档 (*.txt)|*.txt"
        OpenFileDialog1.RestoreDirectory = True
        OpenFileDialog1.FileName = ""
        OpenFileDialog1.Title = "导入域名列表"
        OpenFileDialog1.ShowDialog()
        Dim temp As String = ""
        If OpenFileDialog1.FileName <> "" Then
            temp = My.Computer.FileSystem.ReadAllText(OpenFileDialog1.FileName, System.Text.Encoding.Default)
            temp = temp.Replace(vbCrLf, "剑")
            Dim list() As String = temp.Split("剑")
            Dim url As String
            Dim x1 As Int32
            For x As Int32 = 0 To UBound(list)
                If list(x).Length > 2 Then
                    url = list(x)
                    x1 = url.IndexOf(" ")
                    If x1 > 0 Then url = url.Substring(0, x1)
                    If InStr(url, "http://") = 0 Then url = "http://" & url
                    If sender.name = "Button11" Then
                        ListBox1.Items.Add(url)
                    Else
                        ListBox4.Items.Add(url)
                    End If
                End If
            Next
        End If
        If sender.name = "Button11" Then
            Label8.Text = "作业数量：" & ListBox1.Items.Count
        Else
            Label14.Text = "作业数量：" & ListBox4.Items.Count
        End If
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        My_Flag = "NO"
        Button3.Enabled = False
    End Sub

    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        If ListBox1.Items.Count = 0 Then MsgBox("请先添加要作业的域名列表", , "提示") : Exit Sub
        If ListBox3.Items.Count = 0 Then MsgBox("请先添加要使用的字典", , "提示") : Exit Sub
        Dim GO As New Threading.Thread(AddressOf ScanUrl_SUB)
        GO.Start()
        Timer_Scan.Start()
        Button6.Enabled = False
        Button7.Enabled = True
        Button9.Enabled = True
        ComboBox1.Enabled = False
        ComboBox2.Enabled = False
        ComboBox3.Enabled = False
    End Sub

    Private Sub ScanUrl_SUB()
        On Error Resume Next
        Dim FilePath As String
        Dim txt As String
        Dim temp As New System.Text.StringBuilder
        For x As Int32 = 0 To ListBox3.Items.Count - 1
            FilePath = Application.StartupPath & "\御剑配置文件\" & ListBox3.Items.Item(x).ToString
            FilePath = FilePath.Replace("-使用", "")
            txt = My.Computer.FileSystem.ReadAllText(FilePath, System.Text.Encoding.Default)
            txt = txt.Replace(vbCrLf, "剑")
            temp.Append(txt)
        Next
        Dim list() As String = temp.ToString.Split("剑")
        Scan_list.Clear()
        For x As Int32 = 0 To UBound(list)
            If list(x).Length > 2 Then
                list(x) = list(x).Replace("//", "/")
                list(x) = list(x).Replace("#", "%23")
                If list(x).IndexOf("/") < 0 Then list(x) = "/" & list(x)
                Scan_list.Add(list(x))
            End If
        Next
        Scan_Index = 0
        Scan_Max = ListBox1.Items.Count * Scan_list.Count
        ProgressBar1.Minimum = 0
        ProgressBar1.Maximum = Scan_Max
        ProgressBar1.Value = 0
        Scan_JD = -1
        Scan_TJ = 0
        Scan_SL = 0
        ListView1.Items.Clear()
        If InStr(ComboBox3.Text, "HEAD") > 0 Then
            Scan_Mode = "HEAD"
        Else
            Scan_Mode = "GET"
        End If
        Scan_Flag = 1 '扫描操作标志，1扫描，0停止，3继续，2暂停
        Scan_TimeOut = Val(ComboBox1.Text) * 1000
        For x As Int32 = 1 To Val(ComboBox2.Text)
            Dim GO As New Threading.Thread(AddressOf StartScan_Sub)
            GO.Start()
        Next
    End Sub
    Private Sub StartScan_Sub()
        On Error Resume Next
        Dim index, Max, UrlMax As Int32
        Dim url As String
        Max = Scan_list.Count - 1
        UrlMax = ListBox1.Items.Count - 1
        For x As Int32 = 0 To Scan_Max - 1
            If Scan_Flag = 0 Then Scan_SL = Scan_Max : Exit For '扫描操作标志，1扫描，0停止，3继续，2暂停
            If Scan_Flag = 2 Then
                Do
                    Threading.Thread.Sleep(500)
                    If Scan_Flag = 3 Then Exit Do
                Loop
            End If
            If Scan_Index > UrlMax Then Exit For
            index = Threading.Interlocked.Increment(Scan_JD)
            If index > Max Then
                SyncLock Scan_TB
                    Scan_JD = -1
                    Scan_Index += 1
                    Continue For
                End SyncLock
            End If
            url = ListBox1.Items.Item(Scan_Index) & Scan_list(index)
            ScanDir(url)
            Threading.Interlocked.Increment(Scan_SL)
            Threading.Interlocked.Increment(Scan_TJ)
        Next
        TextBox1.Text = "扫描信息：正在终止线程..."
    End Sub

    Public Function ScanDir(ByVal url As String) As String  '扫描过程
        Dim item As New ListViewItem()
        item.UseItemStyleForSubItems = False
        If Scan_Flag = 0 Then Exit Function
        Dim x1, x2 As Int32
        x1 = url.LastIndexOf(".")
        x2 = url.LastIndexOf("/")
        If x2 > x1 Then
            '目录
            Try
                Dim MyRequest1 As System.Net.HttpWebRequest
                Dim MyResponse1 As System.Net.HttpWebResponse
                Dim HTTP As Int32
                MyRequest1 = CType(Net.WebRequest.Create(url), Net.HttpWebRequest)
                MyRequest1.Method = Scan_Mode
                MyRequest1.ServicePoint.ConnectionLimit = 256
                MyRequest1.AllowAutoRedirect = False
                MyRequest1.Timeout = Scan_TimeOut
                MyRequest1.ReadWriteTimeout = Scan_TimeOut
                MyRequest1.Proxy = Nothing
                MyResponse1 = CType(MyRequest1.GetResponse(), Net.HttpWebResponse)
                MyRequest1.Abort()
                HTTP = MyResponse1.StatusCode
                If HTTP = 200 And MyResponse1.ResponseUri.ToString = url Then
                    If ScanDir2(url.Substring(0, url.Length - 1)) > 200 Then
                        item.Text = ListView1.Items.Count + 1
                        item.SubItems.Add(url)
                        item.SubItems.Add("200")
                        item.SubItems(1).ForeColor = Color.LimeGreen
                        item.SubItems(2).ForeColor = Color.LimeGreen
                        ListView1.Items.Add(item)
                    End If
                End If
                If HTTP > 200 Then
                    If CheckBox_300.Checked Then
                        item.Text = ListView1.Items.Count + 1
                        item.SubItems.Add(url)
                        item.SubItems.Add(HTTP)
                        ListView1.Items.Add(item)
                    End If
                End If
                MyResponse1.Close()
            Catch ex As Exception
                If InStr(ex.Message, "403") > 0 Then
                    If CheckBox_403.Checked Then
                        item.Text = ListView1.Items.Count + 1
                        item.SubItems.Add(url)
                        item.SubItems.Add("403")
                        item.SubItems(1).ForeColor = Color.Red
                        item.SubItems(2).ForeColor = Color.Red
                        ListView1.Items.Add(item)
                    End If
                End If
            End Try
        Else
            '文件
            Try
                Dim FileRequest1 As System.Net.HttpWebRequest
                Dim FileResponse1 As System.Net.HttpWebResponse
                FileRequest1 = CType(Net.WebRequest.Create(url), Net.HttpWebRequest)
                FileRequest1.Method = Scan_Mode
                FileRequest1.ServicePoint.ConnectionLimit = 256
                FileRequest1.AllowAutoRedirect = False
                FileRequest1.Timeout = Scan_TimeOut
                FileRequest1.ReadWriteTimeout = Scan_TimeOut
                FileRequest1.Proxy = Nothing
                FileResponse1 = CType(FileRequest1.GetResponse(), Net.HttpWebResponse)
                FileRequest1.Abort()
                If FileResponse1.StatusCode = 200 Then
                    Dim x As Int32 = url.LastIndexOf("/") + 1
                    url = url.Insert(x, "%20")
                    Dim filecode As Int32 = ScanDir3(url)
                    If filecode > 0 And filecode <> 200 Then
                        item.Text = ListView1.Items.Count + 1
                        url = url.Replace("%20", "")
                        item.SubItems.Add(url)
                        item.SubItems.Add("200")
                        item.SubItems(1).ForeColor = Color.LimeGreen
                        item.SubItems(2).ForeColor = Color.LimeGreen
                        ListView1.Items.Add(item)
                    End If
                End If
                FileResponse1.Close()
            Catch ex As Exception
                If InStr(ex.Message, "403") > 0 Then
                    If CheckBox_403.Checked Then
                        item.Text = ListView1.Items.Count + 1
                        item.SubItems.Add(url)
                        item.SubItems.Add("403")
                        item.SubItems(1).ForeColor = Color.Red
                        item.SubItems(2).ForeColor = Color.Red
                        ListView1.Items.Add(item)
                    End If
                End If
            End Try
        End If
        url = url.Replace("%20", "")
        TextBox1.Text = "扫描信息：" & url
    End Function

    Private Function ScanDir2(ByVal url As String) As Int32
        Try
            Dim MyRequest2 As System.Net.HttpWebRequest
            Dim MyResponse2 As System.Net.HttpWebResponse
            MyRequest2 = CType(Net.WebRequest.Create(url), Net.HttpWebRequest)
            MyRequest2.Method = Scan_Mode
            MyRequest2.ServicePoint.ConnectionLimit = 256
            MyRequest2.Proxy = Nothing
            MyRequest2.AllowAutoRedirect = False
            MyRequest2.Timeout = Scan_TimeOut
            MyRequest2.ReadWriteTimeout = Scan_TimeOut
            MyResponse2 = CType(MyRequest2.GetResponse(), Net.HttpWebResponse)
            MyRequest2.Abort()
            Return MyResponse2.StatusCode
            MyResponse2.Close()
        Catch ex As Exception
            Return 0
        End Try
    End Function

    Private Function ScanDir3(ByVal url As String) As Int32
        Try
            Dim MyRequest2 As System.Net.HttpWebRequest
            Dim MyResponse2 As System.Net.HttpWebResponse
            MyRequest2 = CType(Net.WebRequest.Create(url), Net.HttpWebRequest)
            MyRequest2.Method = Scan_Mode
            MyRequest2.ServicePoint.ConnectionLimit = 256
            MyRequest2.Proxy = Nothing
            MyRequest2.AllowAutoRedirect = False
            MyRequest2.Timeout = Scan_TimeOut
            MyRequest2.ReadWriteTimeout = Scan_TimeOut
            MyResponse2 = CType(MyRequest2.GetResponse(), Net.HttpWebResponse)
            MyRequest2.Abort()
            Return MyResponse2.StatusCode
            MyResponse2.Close()
        Catch ex As Exception
            If InStr(ex.Message, "404") > 0 Then Return 404
        End Try
    End Function

    Private Sub Timer_Scan_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer_Scan.Tick
        On Error Resume Next
        Label10.Text = "扫描速度：" & Scan_TJ & "/每秒"
        Scan_TJ = 0
        ProgressBar1.Value = Scan_SL
        If Scan_SL = Scan_Max Then
            TextBox1.Text = "扫描信息：扫描完成..."
            Label10.Text = "扫描速度：0/每秒"
            ProgressBar1.Value = Scan_Max
            Button6.Enabled = True
            Button7.Enabled = False
            Button8.Enabled = False
            Button9.Enabled = False
            ComboBox1.Enabled = True
            ComboBox2.Enabled = True
            ComboBox3.Enabled = True
            Timer_Scan.Stop()
        End If
    End Sub

    Private Sub Button7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.Click
        Scan_Flag = 0 '扫描操作标志，1扫描，0停止，3继续，2暂停
        Button7.Enabled = False
        Button8.Enabled = False
        Button9.Enabled = False
    End Sub

    Private Sub Button8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.Click
        Scan_Flag = 3 '扫描操作标志，1扫描，0停止，3继续，2暂停
        Button7.Enabled = True
        Button8.Enabled = False
        Button9.Enabled = True
    End Sub

    Private Sub Button9_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button9.Click
        Scan_Flag = 2 '扫描操作标志，1扫描，0停止，3继续，2暂停
        Button7.Enabled = False
        Button8.Enabled = True
        Button9.Enabled = False
    End Sub

    Private Sub ListView_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListView1.MouseDoubleClick, ListView2.MouseDoubleClick
        Try
            If sender.name = "ListView1" Then
                Process.Start(ListView1.SelectedItems(0).SubItems(1).Text)
            Else
                Process.Start(ListView2.SelectedItems(0).SubItems(1).Text)
            End If
        Catch ex As Exception
        End Try
    End Sub

    Private Sub ListView1_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListView1.MouseUp, ListView2.MouseUp
        CD = sender.name
        If sender.name = "ListView1" Then
            If e.Button = 2097152 And ListView1.Items.Count > 0 Then
                ContextMenuStrip2.Show(Control.MousePosition.X, Control.MousePosition.Y)
            End If
        Else
            If e.Button = 2097152 And ListView2.Items.Count > 0 Then
                ContextMenuStrip2.Show(Control.MousePosition.X, Control.MousePosition.Y)
            End If
        End If

    End Sub

    Private Sub 打开网址ToolStripMenuItem1_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 打开网址ToolStripMenuItem1.Click
        Try
            If CD = "ListView1" Then
                Process.Start(ListView1.SelectedItems(0).SubItems(1).Text)
            Else
                Process.Start(ListView2.SelectedItems(0).SubItems(1).Text)
            End If
        Catch ex As Exception
        End Try
    End Sub

    Private Sub 复制网址ToolStripMenuItem1_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 复制网址ToolStripMenuItem1.Click
        If CD = "ListView1" Then
            My.Computer.Clipboard.SetText(ListView1.SelectedItems(0).SubItems(1).Text)
        Else
            My.Computer.Clipboard.SetText(ListView2.SelectedItems(0).SubItems(1).Text)
        End If
    End Sub

    Private Sub 导出网址ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 导出网址ToolStripMenuItem.Click
        On Error Resume Next
        Dim FileName As String
        If CD = "ListView1" Then
            FileName = "后台列表.txt"
        Else
            FileName = "注入点列表.txt"
        End If

        SaveFileDialog1.Filter = "所有文件 (*.*)|*.*|txt文档 (*.txt)|*.txt"
        SaveFileDialog1.RestoreDirectory = True
        SaveFileDialog1.Title = "保存内容"
        SaveFileDialog1.FileName = FileName
        SaveFileDialog1.ShowDialog()
        Dim FileNumber As Int32 = FreeFile()
        If SaveFileDialog1.FileName <> FileName Then
            FileOpen(FileNumber, SaveFileDialog1.FileName, OpenMode.Output)
            If CD = "ListView1" Then
                For x As Int32 = 0 To ListView1.Items.Count - 1
                    Print(FileNumber, ListView1.Items.Item(x).SubItems(1).Text & Space(8) & ListView1.Items.Item(x).SubItems(2).Text & vbCrLf)
                Next
            Else
                For x As Int32 = 0 To ListView2.Items.Count - 1
                    Print(FileNumber, ListView2.Items.Item(x).SubItems.Item(1).Text & vbCrLf)
                Next
            End If
            FileClose(FileNumber)
            MsgBox("已经成功导出数据到" & SaveFileDialog1.FileName, , "提示")
        End If
    End Sub

    Private Sub Button12_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button12.Click, Button13.Click
        If sender.name = "Button12" Then
            ListBox1.Items.Clear()
            Label8.Text = "作业数量：" & ListBox1.Items.Count
        Else
            ListBox4.Items.Clear()
            Label14.Text = "作业数量：" & ListBox4.Items.Count
        End If
    End Sub

    Private Sub LOCK_KeyPress(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyPressEventArgs) Handles ComboBox5.KeyPress, ComboBox4.KeyPress, ComboBox3.KeyPress, ComboBox2.KeyPress, ComboBox1.KeyPress
        e.Handled = True
    End Sub

    Private Sub Button18_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button18.Click
        If ListBox4.Items.Count > 0 Then
            SQL_i = -1
            SQL_JD = 0
            SQL_Task = 0
            ListBox5.Items.Clear()
            ListView2.Items.Clear()
            Sort_KG = False
            SQL_Exit = False
            SQL_Sort.Clear()
            ProgressBar2.Minimum = 0
            ProgressBar2.Maximum = ListBox4.Items.Count
            ProgressBar2.Value = 0
            Label17.Text = "已经采集的地址：0"
            Label18.Text = "可能存在注入的地址：0"
            TextBox7.Text = "检测信息：正在等待采集线程结束..."
            For x As Int32 = 1 To Val(ComboBox5.Text)
                Dim GO As New Threading.Thread(AddressOf SQL_GET_URL)
                GO.IsBackground = True
                GO.Start()
                SQL_Task += 1
            Next
            Button18.Enabled = False
            Button19.Enabled = True
        Else
            MsgBox("作业列表没有数据", , "提示")
        End If
        Timer_SQL.Start()
    End Sub

    Private Sub SQL_GET_URL()
        On Error Resume Next
        Dim Max As Int32 = ListBox4.Items.Count - 1
        Dim index As Int32
        Dim url As String
        For x As Int32 = 0 To Max
            If SQL_Exit Then
                TextBox6.Text = "采集信息：正在结束采集线程..."
                Exit For
            End If
            index = Threading.Interlocked.Increment(SQL_i)
            If index > Max Then Exit For
            url = ListBox4.Items.Item(index)
            Get_SQL_HTML(url)
            If SQL_Exit = False Then
                TextBox6.Text = "采集信息：" & url
                Threading.Interlocked.Increment(SQL_JD)
            End If
            Label17.Text = "已经采集的地址：" & ListBox5.Items.Count
            ProgressBar2.Value = SQL_JD
        Next
        Threading.Interlocked.Decrement(SQL_Task)
        TextBox6.Text = "线程信息[" & SQL_Task.ToString & "]：正在结束采集线程..."
        If SQL_Task < 1 Then
            TextBox6.Text = "采集信息：采集完成..."
        End If
    End Sub

    Private Sub Get_SQL_HTML(ByVal URL As String) '获取HTML源码
        On Error Resume Next
        Dim Request As System.Net.HttpWebRequest
        Dim Response As System.Net.HttpWebResponse
        Dim httpURL As New System.Uri(URL)
        Dim x1, x2, add_i, Max_i, url_x, id_i As Int32
        Dim txt, MyUrl As String
        Dim GetUrl_Arrlist As New List(Of String)
        Dim IDtxt As New List(Of String)
        Dim id_str As String
        Request = CType(Net.WebRequest.Create(httpURL), Net.HttpWebRequest)
        Request.Method = "GET"
        Request.Timeout = 20 * 1000
        Request.ReadWriteTimeout = 20 * 1000
        Request.Proxy = Net.WebRequest.GetSystemWebProxy
        Request.AllowAutoRedirect = True
        Request.AllowWriteStreamBuffering = True
        Request.ServicePoint.ConnectionLimit = 512
        Request.ServicePoint.ConnectionLeaseTimeout = 20 * 1000
        Response = CType(Request.GetResponse(), Net.HttpWebResponse)
        If Response.StatusCode = Net.HttpStatusCode.OK Then
            MyUrl = Response.ResponseUri.ToString.ToLower
            url_x = MyUrl.IndexOf("/", 10) + 1
            MyUrl = MyUrl.Substring(0, url_x)
            Max_i = Val(ComboBox4.Text)
            Dim Encoding As String = Response.CharacterSet.ToUpper
            If InStr(Encoding, "UTF-8") = 0 Then Encoding = "GB2312"
            Dim reader As New IO.StreamReader(Response.GetResponseStream, System.Text.Encoding.GetEncoding(Encoding))
            Dim temp As String = reader.ReadToEnd
            temp = temp.Replace("""", "'")
            reader.Close()
            x1 = 1
            add_i = 1
            SyncLock SQL_TB
                For i As Int32 = 1 To temp.Length
                    If SQL_Exit = True Then
                        Exit Sub
                    End If
                    x1 = InStr(x1, temp, "href='", CompareMethod.Text) + 6
                    If x1 < i Then Exit For
                    If x1 = 0 Then Exit For
                    x2 = InStr(x1, temp, "'", CompareMethod.Text)
                    txt = Mid(temp, x1, x2 - x1)
                    If InStr(txt, "asp?") > 0 Or InStr(txt, "aspx?") > 0 Or InStr(txt, "php?") > 0 Or InStr(txt, "jsp?") > 0 Then
                        If InStr(txt, "http") = 0 Then
                            txt = txt.Replace("../", "/")
                            txt = txt.Replace("./", "/")
                            txt = txt.Replace("//", "/")
                            id_str = MyUrl & "/" & txt
                            id_i = id_str.IndexOf("=") + 1
                            id_str = id_str.Substring(0, id_i)
                            If InStr(txt, "/") = 1 Then txt = Mid(txt, 2, txt.Length - 1)
                            If GetUrl_Arrlist.Contains(MyUrl & "/" & txt) = False And IDtxt.Contains(id_str) = False Then
                                If add_i > Max_i Then Exit For
                                URL = MyUrl & "/" & txt
                                URL = REP_URL(URL)
                                If URL.LastIndexOf("=") <> URL.Length - 1 And URL.IndexOf(";") < 0 And URL.LastIndexOf("?") <> URL.Length - 1 Then
                                    GetUrl_Arrlist.Add(URL)
                                    IDtxt.Add(id_str)
                                    ListBox5.Items.Add(URL)
                                    add_i += 1
                                End If
                            End If
                        End If
                    End If
                    x1 = x2
                Next
            End SyncLock
        End If
        Request.Abort()
        Response.Close()
    End Sub

    Private Function REP_URL(ByVal url As String) As String
        If InStr(url, "https://") Then
            url = url.Replace("https://", "")
            url = url.Replace("./", "/")
            url = url.Replace("../", "/")
            url = url.Replace("//", "/")
            url = "https://" & url
        Else
            url = url.Replace("http://", "")
            url = url.Replace("./", "/")
            url = url.Replace("../", "/")
            url = url.Replace("//", "/")
            url = "http://" & url
        End If
        Return url
    End Function

    Private Sub Timer_SQL_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer_SQL.Tick
        If SQL_Task < 1 Then
            SQL_i = -1
            SQL_JD = 0
            SQL_Task = 0
            ProgressBar2.Minimum = 0
            ProgressBar2.Maximum = ListBox5.Items.Count
            ProgressBar2.Value = 0
            For x As Int32 = 1 To Val(ComboBox5.Text)
                Dim GO As New Threading.Thread(AddressOf SQL_SUB)
                GO.IsBackground = True
                GO.Start()
                Threading.Interlocked.Increment(SQL_Task)
            Next
            Timer_SQL.Stop()
        End If
    End Sub

    Private Sub SQL_SUB()
        On Error Resume Next
        Dim Max As Int32 = ListBox5.Items.Count - 1
        Dim index As Int32
        Dim url As String
        Dim SQL_Num1, SQL_Num2 As String
        Dim Temp, temp1, temp2 As Int32
        SQL_Num1 = TextBox2.Text
        SQL_Num2 = TextBox3.Text
        For x As Int32 = 0 To Max
            If SQL_Exit Then
                Exit For
            End If
            index = Threading.Interlocked.Increment(SQL_i)
            If index > Max Then Exit For
            url = ListBox5.Items.Item(index)
            Temp = 0
            Temp = HEAD_HTML(url)
            If Temp > 10 Then
                'HEAD模式
                temp1 = 0
                temp1 = HEAD_HTML(url & SQL_Num1)
                If temp1 > 10 Then
                    temp2 = 0
                    temp2 = HEAD_HTML(url & SQL_Num2)
                    If Temp = temp1 And Temp <> temp2 Then
                        Dim item As New ListViewItem()
                        item.Text = ListView2.Items.Count + 1
                        item.SubItems.Add(url)
                        SyncLock SQL_TB
                            ListView2.Items.Add(item)
                            SQL_Sort.Add(url)
                        End SyncLock
                    End If
                End If
            Else
                'GET模式
                Temp = 0
                Temp = GET_HTML(url)
                If Temp > 10 Then
                    temp1 = 0
                    temp1 = GET_HTML(url & SQL_Num1)
                    If temp1 > 10 Then
                        temp2 = 0
                        temp2 = GET_HTML(url & SQL_Num2)
                        If Temp = temp1 And Temp <> temp2 Then
                            Dim item As New ListViewItem()
                            item.Text = ListView2.Items.Count + 1
                            item.SubItems.Add(url)
                            SyncLock SQL_TB
                                ListView2.Items.Add(item)
                                SQL_Sort.Add(url)
                            End SyncLock
                        End If
                    End If
                End If
            End If
            Threading.Interlocked.Increment(SQL_JD)
            ProgressBar2.Value = SQL_JD
            TextBox7.Text = "正在检测：" & url
            Label18.Text = "可能存在注入的地址：" & ListView2.Items.Count.ToString
        Next
        If Sort_KG Then Exit Sub
        Threading.Interlocked.Decrement(SQL_Task)
        TextBox7.Text = "线程信息[" & SQL_Task.ToString & "]：正在结束注入检测线程..."
        If SQL_Task < 1 Then
            If SQL_Exit = False Then
                SyncLock SQL_TB
                    Sort_KG = True
                    SQL_Sort.Sort()
                    ListView2.Items.Clear()
                    Dim urlstr As String
                    For x As Int32 = 0 To SQL_Sort.Count - 1
                        urlstr = SQL_Sort.Item(x)
                        Dim item As New ListViewItem()
                        item.UseItemStyleForSubItems = False
                        item.Text = ListView2.Items.Count + 1
                        item.SubItems.Add(urlstr)
                        If InStr(urlstr, ".asp?") = 0 Then
                            item.SubItems(0).ForeColor = Color.Fuchsia
                            item.SubItems(1).ForeColor = Color.Fuchsia
                        End If
                        ListView2.Items.Add(item)
                    Next
                End SyncLock
            End If
            Button18.Enabled = True
            Button19.Enabled = False
            TextBox7.Text = "检测信息：检测完成..."
            Label18.Text = "可能存在注入的地址：" & ListView2.Items.Count.ToString
            ProgressBar2.Value = ProgressBar2.Maximum
        End If
    End Sub

    Private Function HEAD_HTML(ByVal url As String) As Int32 'HEAD取得长度
        Try
            Dim Request As System.Net.HttpWebRequest
            Dim Response As System.Net.HttpWebResponse
            Dim httpURL As New System.Uri(url)
            Request = CType(Net.WebRequest.Create(httpURL), Net.HttpWebRequest)
            Request.Method = "HEAD"
            Request.Timeout = 10 * 1000
            Request.ReadWriteTimeout = 10 * 1000
            Request.AllowAutoRedirect = False
            Request.AllowWriteStreamBuffering = True
            Request.ServicePoint.ConnectionLimit = 512
            Request.ServicePoint.ConnectionLeaseTimeout = 10 * 1000
            Response = CType(Request.GetResponse(), Net.HttpWebResponse)
            If Response.StatusCode = Net.HttpStatusCode.OK And url.ToString.ToLower = Response.ResponseUri.ToString.ToLower Then
                Return Response.ContentLength
            End If
            Request.Abort()
            Response.Close()
        Catch ex As Exception
            Return 0
        End Try
    End Function


    Private Function GET_HTML(ByVal url As String) As Int32 'GET取得长度
        Try
            Dim Request As System.Net.HttpWebRequest
            Dim Response As System.Net.HttpWebResponse
            Dim httpURL As New System.Uri(url)
            Request = CType(Net.WebRequest.Create(httpURL), Net.HttpWebRequest)
            Request.Method = "GET"
            Request.Timeout = 10 * 1000
            Request.ReadWriteTimeout = 10 * 1000
            Request.AllowAutoRedirect = False
            Request.AllowWriteStreamBuffering = False
            Request.ServicePoint.ConnectionLimit = 512
            Request.ServicePoint.ConnectionLeaseTimeout = 10 * 1000
            Response = CType(Request.GetResponse(), Net.HttpWebResponse)
            If Response.StatusCode = Net.HttpStatusCode.OK And url.ToString.ToLower = Response.ResponseUri.ToString.ToLower Then
                Dim Encoding As String = Response.CharacterSet.ToUpper
                If InStr(Encoding, "UTF-8") = 0 Then Encoding = "GB2312"
                Dim reader As New IO.StreamReader(Response.GetResponseStream, System.Text.Encoding.GetEncoding(Encoding))
                Dim temp As String = reader.ReadToEnd
                Return temp.Length
                reader.Close()
            End If
            Request.Abort()
            Response.Close()
        Catch ex As Exception
            Return 0
        End Try
    End Function

    Private Sub Button19_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button19.Click
        SQL_Exit = True
        Button19.Enabled = False
    End Sub

    Private Function GB2312(ByVal Text As String) As String
        Dim i As Long
        Dim x() As Byte
        Dim szRet As String = ""
        x = System.Text.Encoding.Default.GetBytes(Text)
        For i = LBound(x) To UBound(x)
            szRet &= "%" & Hex(x(i))
        Next
        Return szRet
    End Function

    Private Sub TextBox22_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TextBox22.TextChanged
        If TextBox22.Text.Length > 0 Then
            Dim txt As String = TextBox22.Text
            Dim URL_CODE, SQL_CODE, HEX_CODE, ASC_CODE, MD5_32, MD5_16, BE64_CODE, GBK_CODE As String
            URL_CODE = GB2312(txt)
            TextBox23.Text = URL_CODE
            SQL_CODE = URL_CODE.Substring(1, URL_CODE.Length - 1) & "%"
            HEX_CODE = "0x" & SQL_CODE.Replace("%", "")
            SQL_CODE = "0x" & SQL_CODE.Replace("%", "00")
            TextBox24.Text = SQL_CODE
            For x As Int32 = 1 To TextBox22.Text.Length
                ASC_CODE &= Asc(Mid(txt, x, 1)) & " "
                GBK_CODE &= AscW(Mid(txt, x, 1)) & " "
            Next
            TextBox25.Text = GBK_CODE
            TextBox26.Text = HEX_CODE
            U_16(txt)
            U_32(txt)
            TextBox29.Text = encrypt(txt)
            Dim str() As Byte = System.Text.Encoding.Default.GetBytes(txt)
            '加密转换后的字符串
            TextBox30.Text = System.Convert.ToBase64String(str)
        Else
            Textbox23.Text = ""
            Textbox24.Text = ""
            TextBox25.Text = ""
            TextBox26.Text = ""
            TextBox27.Text = ""
            TextBox28.Text = ""
            TextBox29.Text = ""
            TextBox30.Text = ""
        End If
    End Sub

    Private Sub U_16(ByVal txt As String) 'MD5 - 16
        If CheckBox6.Checked Then
            TextBox27.Text = UCase(HASHMD5(txt, 16))
        Else
            TextBox27.Text = HASHMD5(txt, 16)
        End If
    End Sub

    Private Sub U_32(ByVal txt As String) 'MD5 - 32
        If CheckBox5.Checked Then
            TextBox28.Text = UCase(HASHMD5(txt, 32))
        Else
            TextBox28.Text = HASHMD5(txt, 32)
        End If
    End Sub

    Private Function HASHMD5(ByVal strSource As String, ByVal Code As Int16) As String 'MD5加密算法
        On Error Resume Next
        Dim dataToHash As Byte() = (New System.Text.ASCIIEncoding).GetBytes(strSource)
        Dim hashvalue As Byte() = CType(System.Security.Cryptography.CryptoConfig.CreateFromName("MD5"), System.Security.Cryptography.HashAlgorithm).ComputeHash(dataToHash)
        Dim ATR As String = ""
        Dim i As Integer
        Select Case Code
            Case 16      '选择16位字符的加密结果   
                For i = 4 To 11
                    ATR &= Hex(hashvalue(i)).PadLeft(2, "0").ToLower

                Next
            Case 32      '选择32位字符的加密结果   
                For i = 0 To 15
                    ATR &= Hex(hashvalue(i)).PadLeft(2, "0").ToLower
                Next
            Case Else       'Code错误时，返回全部字符串，即32位字符   
                For i = 0 To 15
                    ATR &= Hex(hashvalue(i)).PadLeft(2, "0").ToLower
                Next
        End Select
        Return ATR
    End Function


    Public Function encrypt(ByVal ecode As String)   '雷池加密函数
        Dim texts As String
        Dim i As Int32
        For i = 1 To Len(ecode)
            texts = texts & Chr(Asc(Mid(ecode, i, 2)) + i)
        Next
        encrypt = texts
    End Function

    Public Function decrypt(ByVal dcode As String)     '雷池解密函数
        Dim texts As String
        Dim i As Int32
        For i = 1 To Len(dcode)
            texts = texts & Chr(Asc(Mid(dcode, i, 2)) - i)
        Next
        decrypt = texts
    End Function

    Private Sub CheckBox5_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox5.CheckedChanged
        If CheckBox5.Checked Then
            TextBox28.Text = TextBox28.Text.ToUpper
        Else
            TextBox28.Text = TextBox28.Text.ToLower
        End If
    End Sub

    Private Sub CheckBox6_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox6.CheckedChanged
        If CheckBox6.Checked Then
            TextBox27.Text = TextBox27.Text.ToUpper
        Else
            TextBox27.Text = TextBox27.Text.ToLower
        End If
    End Sub

    Private Sub CheckBox7_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox7.CheckedChanged
        If CheckBox7.Checked Then
            Dim str() As Byte = System.Convert.FromBase64String(TextBox30.Text)
            TextBox30.Text = System.Text.Encoding.Default.GetString(str)
        Else
            Dim str() As Byte = System.Text.Encoding.Default.GetBytes(TextBox22.Text)
            TextBox30.Text = System.Convert.ToBase64String(str)
        End If
    End Sub

    Private Sub CheckBox4_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox4.CheckedChanged
        If CheckBox4.Checked Then
            TextBox29.Text = decrypt(TextBox29.Text)
        Else
            TextBox29.Text = encrypt(TextBox22.Text)
        End If
    End Sub

    Private Sub Button20_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button20.Click
        OpenFileDialog1.Filter = "所有文件 (*.*)|*.*|txt文档 (*.txt)|*.txt"
        OpenFileDialog1.RestoreDirectory = True
        OpenFileDialog1.FileName = ""
        OpenFileDialog1.Title = "导入域名列表"
        OpenFileDialog1.ShowDialog()
        Dim temp As String = ""
        If OpenFileDialog1.FileName <> "" Then
            ListBox6.Items.Clear()
            temp = My.Computer.FileSystem.ReadAllText(OpenFileDialog1.FileName, System.Text.Encoding.Default)
            temp = temp.Replace(vbCrLf, "剑")
            Dim list() As String = temp.Split("剑")
            Dim url As String
            For x As Int32 = 0 To UBound(list)
                If list(x).Length = 32 Then
                    url = list(x)
                    url = MidUrl(url)
                    ListBox6.Items.Add(url)
                End If
            Next
            Label28.Text = "解密序列：" & ListBox6.Items.Count
        End If
    End Sub

    Private Sub Button21_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button21.Click
        If ListBox6.Items.Count > 0 Then
            ListBox7.Items.Clear()
            Label29.Text = "解密结果：" & ListBox7.Items.Count
            ProgressBar3.Minimum = 0
            ProgressBar3.Maximum = ListBox6.Items.Count
            ProgressBar3.Value = 0
            MD5_Index = -1
            MD5_JD = 0
            MD5_Task = 0
            MD5_I = 0
            For x As Int32 = 1 To 20
                Dim GO As New Threading.Thread(AddressOf Get_MD5)
                GO.IsBackground = True
                GO.Start()
                MD5_Task += 1
            Next
            Button21.Enabled = False
        End If
    End Sub

    Private Sub Get_MD5()
        On Error Resume Next
        Dim MAX As Int32 = ListBox6.Items.Count - 1
        Dim index As Int32
        Dim url As String
        Dim txt As String
        Dim X1, X2 As Int32
        For x As Int32 = 0 To MAX
            index = Threading.Interlocked.Increment(MD5_Index)
            If index > MAX Then Exit For
            url = ListBox6.Items.Item(index)
            txt = GetHtmlBody("http://www.md5decrypter.co.uk/feed/api.aspx?" & url, "UTF-8")
            X1 = txt.IndexOf("CDATA[") + 6
            If X1 > 0 Then
                X2 = txt.IndexOf("]")
                If X2 > 0 Then txt = url & "=" & txt.Substring(X1, X2 - X1)
            End If
            SyncLock MD5_TB
                If txt.IndexOf("<?xml") > -1 Then
                    txt = url & "=无法解密"
                End If
                If InStr(txt, "无法解密") = 0 Then MD5_I += 1
                ListBox7.Items.Add(txt)
            End SyncLock
            Label29.Text = "解密结果：" & MD5_I
            Threading.Interlocked.Increment(MD5_JD)
            ProgressBar3.Value = MD5_JD
        Next
        Threading.Interlocked.Decrement(MD5_Task)
        If MD5_Task < 1 Then
            Button21.Enabled = True
        End If
        Label29.Text = "解密结果：" & MD5_I
    End Sub

    Private Sub Button22_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button22.Click
        If ListBox7.Items.Count > 0 Then
            SaveFileDialog1.Filter = "所有文件 (*.*)|*.*|txt文档 (*.txt)|*.txt"
            SaveFileDialog1.RestoreDirectory = True
            SaveFileDialog1.Title = "保存"
            SaveFileDialog1.FileName = "MD5解密列表_32.txt"
            SaveFileDialog1.ShowDialog()
            If SaveFileDialog1.FileName <> "MD5解密列表_32.txt" Then
                FileOpen(1, SaveFileDialog1.FileName, OpenMode.Output)
                For x As Int32 = 0 To ListBox7.Items.Count - 1
                    Print(1, ListBox7.Items.Item(x) & vbCrLf)
                Next
            FileClose(1)
            MsgBox("数据成功导出到" & SaveFileDialog1.FileName, , "提示")
        End If
        End If
    End Sub


    Private Sub MD5_1()
        Dim XML As Object = CreateObject("MSXML2.ServerXMLHTTP")
        Try
            XML.SetTimeOuts(6000, 6000, 6000, 6000)
            XML.Open("POST", "http://www.md5.com.cn/md5reverse", False)
            XML.setRequestHeader("Accept", "text/html, application/xhtml+xml, */*")
            XML.setRequestHeader("Accept-Language", "zh-cn")
            XML.setRequestHeader("If-Modified-Since", "0") '清除缓存
            XML.setRequestHeader("Content-Type", "application/x-www-form-urlencoded")
            XML.Send("md=7fef6171469e80d32c0559f88b377245&submit=MD5+Crack")
            Dim temp As String = System.Text.Encoding.UTF8.GetString(XML.ResponseBody)
            TextBox1.Text = temp
            Dim x1, x2 As Int32
            x1 = temp.IndexOf("style=""color:red;"">") + 19
            If x1 > 0 Then
                x2 = temp.IndexOf("<", x1)
                temp = temp.Substring(x1, x2 - x1)
                TextBox1.Text = "http://www.md5.com.cn" & vbCrLf & "结果：" & temp
            End If
            If InStr(temp, "这是一条收费纪录") > 0 Then
                TextBox1.Text = "http://www.md5.com.cn" & vbCrLf & "结果：这是一条收费纪录"
            End If
            If InStr(temp, "暂没有找到") > 0 Then
                TextBox1.Text = "http://www.md5.com.cn" & vbCrLf & "结果：暂没有找到"
            End If
        Catch ex As Exception
            TextBox1.Text = "http://www.md5.com.cn" & vbCrLf & "结果：查询超时"
        Finally
            XML = Nothing
        End Try
    End Sub

    Private Sub TabPage6_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles TabPage6.Paint
        TextBox9.Text = ""
        TextBox9.Text &= "操作系统：" & My.Computer.Info.OSFullName & vbCrLf & vbCrLf
        TextBox9.Text &= "内部版本：" & My.Computer.Info.OSVersion & vbCrLf & vbCrLf
        TextBox9.Text &= "计算机名：" & My.Computer.Name.ToString & vbCrLf & vbCrLf
        TextBox9.Text &= "公网地址：" & MYtxtIP & vbCrLf & vbCrLf
        Dim Address() As System.Net.IPAddress
        Address = System.Net.Dns.GetHostByName(System.Net.Dns.GetHostName()).AddressList
        TextBox9.Text &= "内网地址：" & Address(0).ToString() & vbCrLf & vbCrLf
        TextBox9.Text &= "当前用户：" & Environment.UserName & vbCrLf & vbCrLf
        TextBox9.Text &= "系统架构：" & My.Computer.Info.OSPlatform & vbCrLf & vbCrLf
        TextBox9.Text &= "并发线程：" & Environment.ProcessorCount & vbCrLf & vbCrLf
        TextBox9.Text &= "物理内存：" & Format(My.Computer.Info.TotalPhysicalMemory / 1024 / 1024 / 1024, "0.00") & "GB" & vbCrLf & vbCrLf
        TextBox9.Text &= "物理可用：" & Format(My.Computer.Info.AvailablePhysicalMemory / 1024 / 1024 / 1024, "0.00") & "GB" & vbCrLf & vbCrLf
        TextBox9.Text &= "虚拟内存：" & Format(My.Computer.Info.TotalVirtualMemory / 1024 / 1024 / 1024, "0.00") & "GB" & vbCrLf & vbCrLf
        TextBox9.Text &= "虚拟可用：" & Format(My.Computer.Info.AvailableVirtualMemory / 1024 / 1024 / 1024, "0.00") & "GB" & vbCrLf & vbCrLf
        TextBox9.Text &= "运行框架：" & Environment.Version.ToString & vbCrLf & vbCrLf
        TextBox9.Text &= "限定操作：" & Environment.SystemDirectory & vbCrLf & vbCrLf
        TextBox9.Text &= "当前进程：" & Application.ExecutablePath & vbCrLf & vbCrLf
        Dim t As Int32 = Environment.TickCount / 1000 '得到总秒数
        Dim TM(3) As Long
        TM(3) = t Mod 60    '秒
        t = t \ 60
        TM(2) = t Mod 60    '分
        t = t \ 60
        TM(1) = t Mod 24    '时
        t = t \ 24
        TM(0) = t           '天
        TextBox9.Text &= "开机时间：" & TM(0) & "天" & TM(1) & "时" & TM(2) & "分" & TM(3) & "秒"
    End Sub

    Private Sub 导出内容ToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles 导出内容ToolStripMenuItem.Click
        On Error Resume Next
        If TreeView1.Nodes.Count > 0 Then
            SaveFileDialog1.Filter = "所有文件 (*.*)|*.*|txt文档 (*.txt)|*.txt"
            SaveFileDialog1.RestoreDirectory = True
            SaveFileDialog1.Title = "保存"
            SaveFileDialog1.FileName = "绑定域名列表.txt"
            SaveFileDialog1.ShowDialog()
            If SaveFileDialog1.FileName <> "绑定域名列表.txt" Then
                FileOpen(1, SaveFileDialog1.FileName, OpenMode.Output)
                Dim url As String
                Dim x As Int32
                For i As Int32 = 0 To TreeView1.Nodes.Count - 1
                    For ii As Int32 = 0 To TreeView1.Nodes(i).Nodes.Count - 1
                        url = (TreeView1.Nodes(i).Nodes(ii).Text)
                        If Not CheckBox3.Checked Then
                            x = InStr(url, " ") - 1
                            If x > 0 Then url = Mid(url, 1, x)
                        End If
                        Print(1, url & vbCrLf)
                    Next
                Next
                FileClose(1)
                MsgBox("数据成功导出到" & SaveFileDialog1.FileName, , "提示")
            End If
        End If
    End Sub

    Private Sub Button23_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button23.Click
        If TextBox8.Text.Length > 0 Then
            Button23.Enabled = False
            TextBox10.Text = "正在查询中..."
            Dim GO As New Threading.Thread(AddressOf MMD5)
            GO.Start()
        End If
    End Sub

    Private Sub MMD5()
        Dim URL As String
        URL = "http://md5.com.cn/index.php/md5reverse/index/md/" & TextBox8.Text & "/submit/MD5%20Crack/"
        Dim TEMP As String = GetHtmlBody(URL, "")
        Dim x1 As Long = InStr(temp, "<b style=""color:red;"">", CompareMethod.Text) + 22
        Dim x2 As Long = InStr(x1, temp, "</b>", CompareMethod.Text)
        If x1 > 3 And x2 > 0 Then
            TextBox10.Text = Mid(temp, x1, x2 - x1)
            If Len(TextBox10.Text) > 16 Then TextBox10.Text = "无法免费查询！"
        Else
            TextBox10.Text = "无法免费查询！"
        End If
        Button23.Enabled = True
    End Sub
End Class

Class TaskThread
    Public IP As String
    Public Sub New(ByVal myip As String)
        IP = myip
    End Sub
End Class
