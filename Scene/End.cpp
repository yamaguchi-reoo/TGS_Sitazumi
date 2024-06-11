#include "End.h"
#include "DxLib.h"

End::End()
{
}

End::~End()
{
}

void End::Initialize()
{

}

void End::Finalize()
{

}

AbstractScene* End::Update()
{
	if (++ExitNum > 179)
	{
		return nullptr;
	}

	return this;
}

void End::Draw()const
{
	SetFontSize(90);
	DrawString(0, 10, "End", 0x00ff00);
}