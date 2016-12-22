using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace checkercom
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            /* 重なり防止 */
            bool CreateWindow;
            string str = System.Diagnostics.Process.GetCurrentProcess().ProcessName;

            System.Threading.Mutex nMutex = new System.Threading.Mutex(true, str, out CreateWindow);

            if (!CreateWindow)
            {
                MessageBox.Show("同じプログラムが動作しています!!", "INFORMATION", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            /* ----------- */
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new main_Form());
        }
    }
}
