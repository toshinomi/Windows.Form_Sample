#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace System::Threading::Tasks;

/// <summary>
/// 共通関数のロジック
/// </summary>
public class ComFunc
{
public:
	static Byte DoubleToByte(double _dValue);
	static Byte LongToByte(long _lValue);
};