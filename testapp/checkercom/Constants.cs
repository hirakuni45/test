using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace checkercom
{
    /** GUI関連定数.
     */
    enum ConstGui
    {
        MAX_NR_ITEM_IN_COMMANDLIST = 2046,//2048,      //< コマンド編集用リストビューの最大項目数 
        MAX_COM_ITEMLIST = 16,
        MAX_PIN_ITEMLIST = 24,
        MAX_DEV_ITEMLIST=64,                  //5種類は固定
        MAX_COMINF_ITEMLIST=MAX_DEV_ITEMLIST*MAX_COM_ITEMLIST,
        MAX_FIRM_MEM=10240
    };


    /** コマンドID.
     */
    enum CommID
    {
        初期値出力,
        定数出力,
        引数出力,
        除算bit出力,
        除加算出力,
        TDS出力,
        Chip出力,
        指定bit出力,
        HW出力,
        Delay,
        繰り返し=31,
        終端=63
    };

    enum PinID
    {
        TWE,
        ALE1,
        ALE2,
        ALE3,
        CLE1,
        CLE2,
        FMS,
        RFU,
        DATA00,
        DATA01,
        DATA02,
        DATA03,
        DATA04,
        DATA05,
        DATA06,
        DATA07,
        DATA08,
        DATA09,
        DATA10,
        DATA11,
        DATA12,
        DATA13,
        DATA14,
        DATA15,
        DATA16,
        DATA17,
        DATA18,
        DATA19,
        DATA20,
        DATA21,
        DATA22,
        DATA23,
        AUX0,
        AUX1,
        AUX2,
        AUX3,
        AUX4,
        AUX5,
        AUX6,
        AUX7
    };
}
