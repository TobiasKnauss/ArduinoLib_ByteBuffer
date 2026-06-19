#include <AUnit.h>

#include <ByteBuffer.h>

//--------------------------------------------------------------------
// - ReadBytesAndMovePtr ()
test (ByteBuffer_LinearBuffer_ReadBytesAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteBuffer* pLinearBuffer = nullptr;
  bool result = ByteBuffer::Create (length, defaultValue, false, pLinearBuffer);
  assertTrue (result);
  uint8_t cache[10];

  //---------- Act & Assert ----------
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (0,  nullptr, false));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (1,  nullptr, false));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (10, nullptr, false));
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (0,  cache, false));
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (2,  cache, false));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (9,  cache, false));
  pLinearBuffer->SetReadPointer (4);
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (9,  cache, false));
  pLinearBuffer->SetReadPointer (0);
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (9,  cache, false));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (10, cache, false));
  pLinearBuffer->SetReadPointer (0);
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (10, cache, false));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)0);
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (0,  cache, true));
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (2,  cache, true));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (9,  cache, true));
  pLinearBuffer->SetReadPointer (0);
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (9,  cache, true));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (10, cache, true));
  pLinearBuffer->SetReadPointer (0);
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (10, cache, true));

  //---------- Cleanup ----------
  delete (pLinearBuffer);
}

//--------------------------------------------------------------------
// - ReadBytesAndMovePtr ()
// - get_pData ()
test (ByteBuffer_LinearBuffer_ReadBytesAndMovePtr_Test2)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteBuffer* pLinearBuffer = nullptr;
  bool result = ByteBuffer::Create (length, defaultValue, false, pLinearBuffer);
  assertTrue (result);

  uint8_t cacheSize = 10;
  uint8_t cache[10];

  //---------- Act & Assert ----------
  memset (&cache, 0x00, sizeof (cache));
  assertTrue (pLinearBuffer->ReadBytesAndMovePtr (3, cache, false));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)3);
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], index < 3 ? defaultValue : 0x00);

  memset (&cache, 0x00, sizeof (cache));
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (7, cache, false));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)3);
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], 0x00);

  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (6, cache, false));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)9);
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], index < 6 ? defaultValue : 0x00);

  //---------- Arrange ----------
  for (uint16_t index = 0; index < length; index++)
    pLinearBuffer->get_pData()[index] = (((index + 7) % length) + 1) * 11;  // -> 88 99 11 22 33 44 55 66 77

  //---------- Act & Assert ----------
  memset (&cache, 0x00, sizeof (cache));
  assertTrue  (pLinearBuffer->SetReadPointer (0));  // -> reading starts at address 0
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (9, cache + 1, false));  // cache: 00 88 99 11 22 33 44 55 66 77
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)9);
  assertEqual (cache[0], 0);
  assertEqual (cache[1], 88);
  assertEqual (cache[2], 99);
  assertEqual (cache[3], 11);
  assertEqual (cache[4], 22);
  assertEqual (cache[5], 33);
  assertEqual (cache[6], 44);
  assertEqual (cache[7], 55);
  assertEqual (cache[8], 66);
  assertEqual (cache[9], 77);

  memset (&cache, 0x00, sizeof (cache));
  assertTrue  (pLinearBuffer->SetReadPointer (5));  // -> reading starts at address 5
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (4, cache + 2, true));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)9);
  assertFalse (pLinearBuffer->ReadBytesAndMovePtr (2, cache    , true));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)9);
  assertTrue  (pLinearBuffer->SetReadPointer (0));
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (2, cache    , true));
  assertTrue  (pLinearBuffer->ReadBytesAndMovePtr (3, cache + 6, true));
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], (uint8_t)((9 - index) * 11));
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)5);

  //---------- Cleanup ----------
  delete (pLinearBuffer);
}

//--------------------------------------------------------------------
// - ReadValueAndMovePtr ()
// - get_pData ()
test (ByteBuffer_LinearBuffer_ReadValueAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteBuffer* pLinearBuffer = nullptr;
  bool result = ByteBuffer::Create (length, defaultValue, false, pLinearBuffer);
  assertTrue (result);

  for (uint16_t index = 0; index < length; index++)
    pLinearBuffer->get_pData ()[index] = (uint8_t)((8 - index) * 11);  // -> 88 77 66 55 44 33 22 11 00  =  0x 58 4D 42 37 2C 21 16 0B 00

  //---------- Act & Assert ----------
  for (uint16_t index = 0; index < length; index++)
  {
    bool valueB;
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), index);
    assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueB));
    assertEqual (valueB, index != length - 1);
  }

  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    uint8_t valueUI8;
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), index);
    assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueUI8));
    assertEqual (valueUI8, (uint8_t)((8 - index) * 11));
  }

  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    int8_t valueI8;
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), index);
    assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueI8));
    assertEqual (valueI8, (int8_t)((8 - index) * 11));
  }

  // 58 4D 42 37 2C 21 16 0B 00
  // [58][4D] --(little-endian => little end of number comes first in memory)--> 0x4D58
  uint16_t valueUI16;
  pLinearBuffer->SetReadPointer (0);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)0); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x4D58);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)2); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x3742);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)4); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x212C);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)6); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x0B16);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)8); assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false));
  pLinearBuffer->SetReadPointer (1);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)1); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x424D);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)3); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x2C37);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)5); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x1621);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)7); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, false)); assertEqual (valueUI16, (uint16_t)0x000B);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)9);

  pLinearBuffer->SetReadPointer (0);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)0); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x584D);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)2); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x4237);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)4); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x2C21);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)6); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x160B);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)8); assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true ));
  pLinearBuffer->SetReadPointer (1);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)1); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x4D42);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)3); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x372C);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)5); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x2116);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)7); assertTrue  (pLinearBuffer->ReadValueAndMovePtr (valueUI16, true )); assertEqual (valueUI16, (uint16_t)0x0B00);
  assertEqual (pLinearBuffer->get_CurrentReadAddress (), (uint16_t)9);

  int16_t valueI16;
  uint16_t expectedUI16[9]     = { 0x4D58, 0x3742, 0x212C, 0x0B16, 0x5800, 0x424D, 0x2C37, 0x1621, 0x000B };
  uint16_t expectedUI16_inv[9] = { 0x584D, 0x4237, 0x2C21, 0x160B, 0x0058, 0x4D42, 0x372C, 0x2116, 0x0B00 };

  uint16_t address = 0;
  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), address);
    if (index == 4)
    {
      assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueI16, false));
      pLinearBuffer->SetReadPointer (1);
    }
    else
    {
      assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueI16, false));
      assertEqual (valueI16, (int16_t)expectedUI16[index]);
    }
    address = (address + 2) % length;
  }

  address = 0;
  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), address);
    if (index == 4)
    {
      assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueI16, true));
      pLinearBuffer->SetReadPointer (1);
    }
    else
    {
      assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueI16, true));
      assertEqual (valueI16, (int16_t)expectedUI16_inv[index]);
    }
    address = (address + 2) % length;
  }

  uint32_t valueUI32;
  int32_t  valueI32;
  uint32_t expectedUI32[9]     = { 0x37424D58, 0x0B16212C, 0x424D5800, 0x16212C37, 0x4D58000B, 0x212C3742, 0x58000B16, 0x2C37424D, 0x000B1621 };
  uint32_t expectedUI32_inv[9] = { 0x584D4237, 0x2C21160B, 0x00584D42, 0x372C2116, 0x0B00584D, 0x42372C21, 0x160B0058, 0x4D42372C, 0x21160B00 };

  address = 0;
  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), address);
    if (pLinearBuffer->get_CurrentReadAddress () + 4 > length)
    {
      assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueUI32, false));
      pLinearBuffer->SetReadPointer ((pLinearBuffer->get_CurrentReadAddress () + 4) % length);
    }
    else
    {
      assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueUI32, false));
      assertEqual (valueUI32, expectedUI32[index]);
    }
    address = (address + 4) % length;
  }

  address = 0;
  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), address);
    if (pLinearBuffer->get_CurrentReadAddress () + 4 > length)
    {
      assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueUI32, true));
      pLinearBuffer->SetReadPointer ((pLinearBuffer->get_CurrentReadAddress () + 4) % length);
    }
    else
    {
      assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueUI32, true));
      assertEqual (valueUI32, expectedUI32_inv[index]);
    }
    address = (address + 4) % length;
  }

  address = 0;
  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), address);
    if (pLinearBuffer->get_CurrentReadAddress () + 4 > length)
    {
      assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueI32, false));
      pLinearBuffer->SetReadPointer ((pLinearBuffer->get_CurrentReadAddress () + 4) % length);
    }
    else
    {
      assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueI32, false));
      assertEqual (valueI32, (int32_t)expectedUI32[index]);
    }
    address = (address + 4) % length;
  }

  address = 0;
  pLinearBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pLinearBuffer->get_CurrentReadAddress (), address);
    if (pLinearBuffer->get_CurrentReadAddress () + 4 > length)
    {
      assertFalse (pLinearBuffer->ReadValueAndMovePtr (valueI32, true));
      pLinearBuffer->SetReadPointer ((pLinearBuffer->get_CurrentReadAddress () + 4) % length);
    }
    else
    {
      assertTrue (pLinearBuffer->ReadValueAndMovePtr (valueI32, true));
      assertEqual (valueI32, (int32_t)expectedUI32_inv[index]);
    }
    address = (address + 4) % length;
  }
}

//--------------------------------------------------------------------
void setup ()
{
  Serial.begin (115200);
  delay (2000);
}

void loop ()
{
  aunit::TestRunner::run ();
}
