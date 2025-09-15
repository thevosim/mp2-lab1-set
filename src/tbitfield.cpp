// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
  if(len < 0) throw "The length cannot be a negative number";
  MemLen = len / (sizeof(TELEM) * 8);
  if(len % (sizeof(TELEM) * 8) != 0) ++MemLen;
  pMem = new TELEM[MemLen];
  BitLen = len;
  for(int i = 0; i < MemLen; ++i) pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for(int i = 0; i < MemLen; ++i) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  BitLen = 0;
  MemLen = 0;
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if(n < 0) throw "The bit number cannot be negative";
  if(n >= BitLen) "The index is too large";
  return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if(n < 0) throw "The bit number cannot be negative";
  if(n >= BitLen) "The index is too large";
  TELEM mask = (1 << (sizeof(TELEM) * 8 - n % (sizeof(TELEM) * 8) - 1));
  return TELEM(mask);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if(n < 0) throw "The bit number cannot be negative";
  if(n >= BitLen) throw "The index is too large";
  TELEM mask = GetMemMask(n);
  pMem[GetMemIndex(n)] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if(n < 0) throw "The bit number cannot be negative";
  if(n >= BitLen) throw "The index is too large";
  TELEM mask = GetMemMask(n);
  pMem[GetMemIndex(n)] &= (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if(n < 0) throw "The bit number cannot be negative";
  if(n >= BitLen) throw "The index is too large";
  TELEM mask = GetMemMask(n);
  TELEM res = (pMem[GetMemIndex(n)] & mask);
  if(res != 0) return 1;
  else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
  if(&bf != this)
  {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for(int i = 0; i < MemLen; ++i) pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if(bf.BitLen != BitLen) return 0;
  for(int i = 0; i < MemLen; ++i) if(pMem[i] != bf.pMem[i]) return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !((*this) == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int MaxBitLen = (BitLen > bf.BitLen ? BitLen : bf.BitLen);
  TBitField res(MaxBitLen);
  int MinLen = (MemLen < bf.MemLen ? MemLen : bf.MemLen);
  for(int i = 0; i < MinLen; ++i) res.pMem[i] = bf.pMem[i] | pMem[i];

  if(MemLen > MinLen) 
    for(int i = MinLen; i < MemLen; ++i)
      res.pMem[i] = pMem[i];
  else
    for(int i = MinLen; i < bf.MemLen; ++i)
      res.pMem[i] = bf.pMem[i];
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int MaxBitLen = (BitLen > bf.BitLen ? BitLen : bf.BitLen);
  TBitField res(MaxBitLen);
  int MinLen = (MemLen < bf.MemLen ? MemLen : bf.MemLen);
  for(int i = 0; i < MinLen; ++i) res.pMem[i] = bf.pMem[i] & pMem[i];

  if(MemLen > MinLen) 
    for(int i = MinLen; i < MemLen; ++i)
      res.pMem[i] = pMem[i];
  else
    for(int i = MinLen; i < bf.MemLen; ++i)
      res.pMem[i] = bf.pMem[i];
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];
    int RightShift = sizeof(TELEM) * 8 - (MemLen * sizeof(TELEM) * 8 - BitLen);
    if(RightShift < sizeof(TELEM) * 8) 
    {
      TELEM mask = (TELEM(~0) >> RightShift);
      res.pMem[MemLen - 1] &= (~mask);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  for(int i = 0; i < bf.MemLen; ++i) istr >> bf.pMem[i];
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for(int i = 0; i < bf.BitLen; ++i) 
  {
    ostr << bf.GetBit(i);
  }
  return ostr;
}
