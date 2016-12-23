#ifndef _CK4_GPIO_H
#define _CK4_GPIO_H

#define INPUT_Now_1		now_input_1._byte			//!< @brief 十字キーの現在の値
#define INPUT_Now_2		now_input_2._byte			//!< @brief 制御キーの現在の値
#define INPUT_Pre_1		pre_input_1._byte			//!< @brief 十字キーの前回の値
#define INPUT_Pre_2		pre_input_2._byte			//!< @brief 制御キーの前回の値

#define INPUT_A1_Now	now_input_1.b0				//!< @brief A1キーの現在の値
#define INPUT_A2_Now	now_input_1.b1				//!< @brief A2キーの現在の値
#define INPUT_A4_Now	now_input_1.b2				//!< @brief A4キーの現在の値
#define INPUT_A3_Now	now_input_1.b3				//!< @brief A3キーの現在の値

#define INPUT_B1_Now	now_input_1.b4				//!< @brief B1キーの現在の値
#define INPUT_B2_Now	now_input_1.b5				//!< @brief B2キーの現在の値
#define INPUT_B4_Now	now_input_1.b6				//!< @brief B4キーの現在の値
#define INPUT_B3_Now	now_input_1.b7				//!< @brief B3キーの現在の値

#define INPUT_Mk_Now	now_input_2.b0				//!< @brief MARKキーの現在の値
#define INPUT_Tg_Now	now_input_2.b1				//!< @brief TARGETキーの現在の値
#define INPUT_Tr_Now	now_input_2.b2				//!< @brief TRACKキーの現在の値
#define INPUT_Pw_Now	now_input_2.b7				//!< @brief 電源キーの現在の値

void gpio_init(void);
void key_scan_proc(void);
void Continue_proc(void);
void vsync_proc(void);

#endif /*_CK4_GPIO_H*/
