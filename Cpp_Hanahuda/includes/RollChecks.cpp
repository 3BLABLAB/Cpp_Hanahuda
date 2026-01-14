# include<RollChecks.h>
void CheckRolls(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	Goko(Mochihuda,Rolls,PreRoll);
	HanamiDeIppai(Mochihuda,Rolls, PreRoll);
	TsukimiDeIppai(Mochihuda,Rolls, PreRoll);
	Inoshikacho(Mochihuda,Rolls, PreRoll);
	Akatan(Mochihuda,Rolls, PreRoll);
	Aotan(Mochihuda,Rolls, PreRoll);
	Tane(Mochihuda,Rolls, PreRoll);//得点変動
	Tann(Mochihuda,Rolls, PreRoll);//得点変動
	Kasu(Mochihuda,Rolls, PreRoll);//得点変動
}

void Goko(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count = 0;
	if (Mochihuda[0][0]) count++;
	if (Mochihuda[2][0]) count++;
	if (Mochihuda[7][0]) count++;
	if (Mochihuda[11][0]) count++;
	if (count == 2)PreRoll = true;
	else if (count == 3) Rolls[0] = U"Sanko";
	else if (count == 4)
	{
		if (Mochihuda[10][0]) Rolls[0] = U"AmeShiko";
		else Rolls[0] = U"Shiko";
	}
	else if (count == 5) Rolls[0] = U"Goko";
}

void HanamiDeIppai(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	if (Mochihuda[2][0] && Mochihuda[8][0])
	{
		Rolls[1] = U"HanamiDeIppai";
	}
	else if (Mochihuda[2][0] || Mochihuda[8][0]) {
		PreRoll = true;
	}
}

void TsukimiDeIppai(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	if (Mochihuda[7][0] && Mochihuda[8][0])
	{
		Rolls[2] = U"TsukimiDeIppai";
	}
	else if (Mochihuda[7][0] || Mochihuda[8][0]) {
		PreRoll = true;
	}
}

void Inoshikacho(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count = 0;
	if (Mochihuda[5][0]) count++;
	if (Mochihuda[6][0]) count++;
	if (Mochihuda[9][0]) count++;

	if (count == 2)PreRoll = true;
	else if(count==3){
		Rolls[3] = U"Inoshikacho";
	}
}

void Akatan(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count = 0;
	if (Mochihuda[0][1])count++;
	if (Mochihuda[1][1])count++;
	if (Mochihuda[2][1])count++;
	if (count == 2)PreRoll = true;
	else if(count==3){
		Rolls[4] = U"Akatan";
	}
}

void Aotan(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count = 0;
	if (Mochihuda[5][1])count++;
	if (Mochihuda[8][1])count++;
	if (Mochihuda[9][1])count++;
	if (count == 2)PreRoll = true;
	else if (count == 3) {
		Rolls[5] = U"Aotan";
	}
}

void Tane(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count = 0;
	if (Mochihuda[1][0]) count++;
	if (Mochihuda[3][0]) count++;
	if (Mochihuda[4][0]) count++;
	if (Mochihuda[5][0]) count++;
	if (Mochihuda[6][0]) count++;
	if (Mochihuda[7][1]) count++;
	if (Mochihuda[8][1]) count++;
	if (Mochihuda[9][0]) count++;
	if (Mochihuda[10][1]) count++;
	if (count == 4)PreRoll = true;
	else if (count >= 5)
	{
		Rolls[6] = U"Tane" + Format(count - 4);
	}
}

void Tann(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count = 0;
	if (Mochihuda[0][1]) count++;
	if (Mochihuda[1][1]) count++;
	if (Mochihuda[2][1]) count++;
	if (Mochihuda[3][1]) count++;
	if (Mochihuda[4][1]) count++;
	if (Mochihuda[5][1]) count++;
	if (Mochihuda[6][1]) count++;
	if (Mochihuda[8][1]) count++;
	if (Mochihuda[9][1]) count++;
	if (Mochihuda[10][2]) count++;
	if (count == 4)PreRoll = true;
	else if (count >= 5)
	{
		Rolls[7] = U"Tan" + Format(count - 4);
	}
}

void Kasu(const HudaFlagTable& Mochihuda, std::vector <String>& Rolls, bool& PreRoll) {
	int count=0;
	for (int i = 0; i < 10; i++)
	{
		//1～11月
		for (int j = 2; j < 4; j++)
		{
			if (Mochihuda[i][j]) count++;
		}
	}
	//1月
	if (Mochihuda[10][3]) count++;
	//12月
	for (int j = 1; j < 4; j++)
	{
		if (Mochihuda[11][j]) count++;
	}
	if (count == 9)PreRoll = true;
	else if (count >= 10)
	{
		Rolls[8] = U"Kasu" + Format(count - 9);
	}
}
