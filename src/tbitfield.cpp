// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
  MemLen = len / sizeof(TELEM);
  if(len % sizeof(TELEM) != 0) ++MemLen;
  pMem = new TELEM[MemLen];
  BitLen = MemLen * sizeof(TELEM);
  for(int i = 0; i < MemLen; ++i) pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  delete[] pMem;
  pMem = new TELEM[BitLen];
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
  return n / sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  int mask = (1 << (sizeof(TELEM) - n % sizeof(TELEM)) - 1);
  return TELEM(mask);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  int mask = GetMemMask(n);
  pMem[GetMemIndex(n)] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
  int mask = GetMemMask(n);
  pMem[GetMemIndex(n)] &= (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  int mask = GetMemMask(n);
  int res = (pMem[GetMemIndex(n)] & mask);
  if(res != 0) return 1;
  return res;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
  if(&bf != this)
  {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[BitLen];
    for(int i = 0; i < BitLen; ++i) pMem[i] = bf.pMem[i];
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
  if(MemLen > bf.MemLen)
  {
    TBitField res(MemLen);
    for(int i = 0; i < MemLen - bf.MemLen; ++i) res.pMem[i] = pMem[i];
    for(int i = 0; i < bf.MemLen; ++i)
      res.pMem[i + (MemLen - bf.MemLen)] = (pMem[i + (MemLen - bf.MemLen)] | bf.pMem[i]);
    return res;
  }
  else
  {
    TBitField res(MemLen);
    for(int i = 0; i < bf.MemLen - MemLen; ++i) res.pMem[i] = bf.pMem[i];
    for(int i = 0; i < MemLen; ++i)
      res.pMem[i + (bf.MemLen - MemLen)] = (pMem[i + (bf.MemLen - MemLen)] | bf.pMem[i]);
    return res;
  } 
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return TBitField(0);
}

TBitField TBitField::operator~(void) // отрицание
{
    return TBitField(0);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
