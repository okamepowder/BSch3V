#pragma once

// �ȉ��̃}�N���́A�Œ���K�v�ȃv���b�g�t�H�[�����`���܂��B�Œ���K�v�ȃv���b�g�t�H�[���Ƃ́A
// �A�v���P�[�V���������s���邽�߂ɕK�v�ȋ@�\��������ł��Â��o�[�W������ Windows �� Internet Explorer �Ȃ�
// �������܂��B�����̃}�N���́A�w�肵���o�[�W�����ƁA����ȑO�̃o�[�W�����̃v���b�g�t�H�[����ŗ��p�ł��邷�ׂĂ̋@�\��L���ɂ��邱�Ƃɂ���� 
// ���삵�܂��B

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER                  // Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define WINVER 0x0501           // ����� Windows XP �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINNT            // Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINNT 0x0501     // ����� Windows XP �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINDOWS          // Windows ME �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINDOWS 0x0500   // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE               // IE 6.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_IE 0x0600        // ����� IE 6.0  �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif
