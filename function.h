#ifdef FUNCTION
// function.cpp����C���N���[�h���ꂽ�ꍇ

#else
// function.cpp�ȊO����C���N���[�h���ꂽ�ꍇ

//	A����B�ւ̊p�x���v�Z����B
extern float GetAngle(float Xa , float Ya , float Xb , float Yb);

//	�p�x�ƈړ���������AX������Y�������v�Z����B
extern void GetVector(float move , float RadAngle , float *X , float *Y);

//	�����蔻�������Bsize�͓����蔻��̃T�C�Y
//	�߂�l��TRUE�����������Ƃ��AFALSE���O�ꂽ�Ƃ�
extern bool isHit(float xA , float yA , float sizeA , float xB , float yB , float sizeB);

#endif

