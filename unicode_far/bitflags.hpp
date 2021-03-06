#pragma once

/*
bitflags.hpp

�����
*/
/*
Copyright � 1996 Eugene Roshal
Copyright � 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
template<class T>
class TBitFlags
{
public:
		TBitFlags():m_Flags(0) {}
		TBitFlags(const T& Flags):m_Flags(Flags) {}

		const T& Flags() const { return m_Flags; }
		// ���������� ����� ������
		const T& Set(const T& FlagsToSet) { m_Flags |= FlagsToSet; return m_Flags;}
		// �������� ����� ������
		const T& Clear(const T& FlagsToClear) { m_Flags &=~ FlagsToClear; return m_Flags; }
		// ��������� ����� ������
		bool Check(const T& FlagsToCheck) const { return m_Flags & FlagsToCheck? true : false; }
		// �������� ��������� ������ ������ � ����������� �� Status
		const T& Change(const T& FlagsToChange, bool set) { return set? Set(FlagsToChange) : Clear(FlagsToChange); }
		// ������������� ��������� ������
		const T& Swap(const T& FlagsToSwap) { return Check(FlagsToSwap)? Clear(FlagsToSwap) : Set(FlagsToSwap); }
		//�������� ��� �����
		void ClearAll() { m_Flags = 0; }
private:
		T m_Flags;};

typedef TBitFlags<DWORD> BitFlags;
