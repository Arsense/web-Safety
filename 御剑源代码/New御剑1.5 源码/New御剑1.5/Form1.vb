Public Class Form1

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        Environment.Exit(0)
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim temp As String = ""
        Button1.Enabled = False
        If TextBox1.Text.Length < 1 Then MsgBox("请输入帐号") : Exit Sub
        If TextBox2.Text.Length < 1 Then MsgBox("请输入密码") : Exit Sub
        Dim url As String = "http://bbs.hake.cc/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&inajax=1&username=" & GB2312(TextBox1.Text) & "&password=" & TextBox2.Text & "&quickforward=yes&handlekey=ls&questionid=0&answer=" & TextBox3.Text
        url = "http://bbs.hake.cc/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&inajax=1&username=" & GB2312(TextBox1.Text) & "&password=" & TextBox2.Text & "&quickforward=yes&handlekey=ls&questionid=0&answer=" & TextBox3.Text
        WebBrowser1.Navigate(url)
        Do
            If WebBrowser1.ReadyState = WebBrowserReadyState.Complete Then
                WebBrowser1.Navigate(url)
                Do
                    If WebBrowser1.ReadyState = WebBrowserReadyState.Complete Then Exit Do
                    Threading.Thread.Sleep(50)
                    Application.DoEvents()
                Loop
                Exit Do
            End If
            Threading.Thread.Sleep(50)
            Application.DoEvents()
        Loop
        temp = WebBrowser1.DocumentText
        If temp.IndexOf("欢迎") > -1 Then
            If CheckBox1.Checked Then
                My.Computer.FileSystem.WriteAllText(Application.StartupPath & "\御剑配置文件\user.db", TextBox1.Text & "|" & TextBox2.Text & "|" & TextBox3.Text, False, System.Text.Encoding.Default)
            End If
            FrmMain.Label35.Text = TextBox1.Text
            FrmMain.Show()
            Me.Hide()
        Else
            MsgBox("帐号或者密码错误！")
        End If
        Button1.Enabled = True
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

    Private Function GetHtmlBody(ByVal URL As String, ByVal Code As String) As String '获取HTML源码
        On Error Resume Next
        Dim Request As System.Net.HttpWebRequest
        Dim Response As System.Net.HttpWebResponse
        Dim httpURL As New System.Uri(URL)
        Request = CType(Net.WebRequest.Create(httpURL), Net.HttpWebRequest)
        Request.Method = "GET"
        Request.Timeout = 20 * 1000
        Request.AllowAutoRedirect = True
        Request.AllowWriteStreamBuffering = False
        Request.ReadWriteTimeout = 30 * 1000
        Request.ServicePoint.ConnectionLimit = 512
        Request.MaximumAutomaticRedirections = 50
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

    Private Sub Form1_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        Environment.Exit(0)
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        On Error Resume Next
        If Dir(Application.StartupPath & "\御剑配置文件\user.db") <> "" Then
            CheckBox1.Checked = True
            Dim str As String = My.Computer.FileSystem.ReadAllText(Application.StartupPath & "\御剑配置文件\user.db", System.Text.Encoding.Default)
            Dim user, pwd, aq As String
            Dim i1, i2 As Int32
            i1 = str.IndexOf("|")
            i2 = str.IndexOf("|", i1 + 1)
            If i1 > -1 Then
                user = str.Substring(0, i1)
                i1 += 1
                pwd = str.Substring(i1, i2 - i1)
                i2 += 1
                aq = str.Substring(i2, str.Length - i2)
                TextBox1.Text = user
                TextBox2.Text = pwd
                TextBox3.Text = aq
            End If
        End If
    End Sub

    Private Sub CheckBox1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox1.CheckedChanged

    End Sub
End Class