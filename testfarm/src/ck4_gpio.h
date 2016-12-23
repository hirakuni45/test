#ifndef _CK4_GPIO_H
#define _CK4_GPIO_H

#define INPUT_Now_1		now_input_1._byte			//!< @brief �\���L�[�̌��݂̒l
#define INPUT_Now_2		now_input_2._byte			//!< @brief ����L�[�̌��݂̒l
#define INPUT_Pre_1		pre_input_1._byte			//!< @brief �\���L�[�̑O��̒l
#define INPUT_Pre_2		pre_input_2._byte			//!< @brief ����L�[�̑O��̒l

#define INPUT_A1_Now	now_input_1.b0				//!< @brief A1�L�[�̌��݂̒l
#define INPUT_A2_Now	now_input_1.b1				//!< @brief A2�L�[�̌��݂̒l
#define INPUT_A4_Now	now_input_1.b2				//!< @brief A4�L�[�̌��݂̒l
#define INPUT_A3_Now	now_input_1.b3				//!< @brief A3�L�[�̌��݂̒l

#define INPUT_B1_Now	now_input_1.b4				//!< @brief B1�L�[�̌��݂̒l
#define INPUT_B2_Now	now_input_1.b5				//!< @brief B2�L�[�̌��݂̒l
#define INPUT_B4_Now	now_input_1.b6				//!< @brief B4�L�[�̌��݂̒l
#define INPUT_B3_Now	now_input_1.b7				//!< @brief B3�L�[�̌��݂̒l

#define INPUT_Mk_Now	now_input_2.b0				//!< @brief MARK�L�[�̌��݂̒l
#define INPUT_Tg_Now	now_input_2.b1				//!< @brief TARGET�L�[�̌��݂̒l
#define INPUT_Tr_Now	now_input_2.b2				//!< @brief TRACK�L�[�̌��݂̒l
#define INPUT_Pw_Now	now_input_2.b7				//!< @brief �d���L�[�̌��݂̒l

void gpio_init(void);
void key_scan_proc(void);
void Continue_proc(void);
void vsync_proc(void);

#endif /*_CK4_GPIO_H*/
