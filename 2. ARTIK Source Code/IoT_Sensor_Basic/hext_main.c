/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: HexT_main.h
* Author: sj.yang
* Release date: 2018/09/18
* Version: 1.2
*
****************************************************************************/

#include <tinyara/config.h>
#include <apps/shell/tash.h>
#include <stdio.h>
#include "hext.h"
#include "game.h"


int main(int argc, FAR char *argv[])
{
	tash_cmd_install("hext",hext_main,TASH_EXECMD_SYNC);
	tash_cmd_install("game", game_main, TASH_EXECMD_SYNC);
	return 0;
}


