#include <AUnit.h>

#include <ByteRingBuffer.h>

//--------------------------------------------------------------------
// Test of the functions
// - Create ()
test (ByteRingBuffer_Create_Test1)
{
  //---------- Arrange ----------
  uint8_t value = 0;
  ByteRingBuffer* pRingBuffer = (ByteRingBuffer*)&value;

  //---------- Act ----------
  bool result = ByteRingBuffer::Create (20, 0xFF, pRingBuffer);

  //---------- Assert ----------
  assertFalse (result);
  assertEqual (pRingBuffer, &value);
}

//--------------------------------------------------------------------
// Test of the functions
// - Create ()
test (ByteRingBuffer_Create_Test2)
{
  //---------- Arrange ----------
  ByteRingBuffer* pRingBuffer = nullptr;

  //---------- Act ----------
  bool result = ByteRingBuffer::Create (0, 0xFF, pRingBuffer);

  //---------- Assert ----------
  assertFalse (result);
  assertEqual (pRingBuffer, nullptr);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - Create ()
// - get_Length ()
test (ByteRingBuffer_Create_Test3)
{
  //---------- Arrange ----------
  uint16_t length = 20;
  ByteRingBuffer* pRingBuffer = nullptr;

  //---------- Act ----------
  bool result = ByteRingBuffer::Create (length, 0xFF, pRingBuffer);

  //---------- Assert ----------
  assertTrue (result);
  assertNotEqual (pRingBuffer, nullptr);
  assertEqual (pRingBuffer->get_Length (), length);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - ReadByteAndMovePtr ()
// - MoveReadPointer ()
// - SetReadPointer ()
// - get_CurrentReadAddress ()
test (ByteRingBuffer_ReadByteAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint8_t newValue1 = 0x12;
  uint16_t length = 5;
  ByteRingBuffer* pRingBuffer = nullptr;
  bool result = ByteRingBuffer::Create (length, defaultValue, pRingBuffer);
  assertTrue (result);

  //---------- Act & Assert ----------
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);
  for (uint16_t index = 0; index < 2 * length; index++)
  {
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
    assertEqual (pRingBuffer->get_CurrentReadAddress (), (index + 1) % length);
    assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);
  }
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);

  //---------- Arrange ----------
  pRingBuffer->WriteByteAndMovePtr (newValue1);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)1);

  //---------- Act & Assert ----------
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);

  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)1);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)3);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)4);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);

  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)1);
  pRingBuffer->MoveReadPointer ();
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)2);
  pRingBuffer->MoveReadPointer ();
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)3);
  pRingBuffer->MoveReadPointer (2);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);

  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)1);
  pRingBuffer->SetReadPointer (0);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);

  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  pRingBuffer->MoveReadPointer (2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);
  pRingBuffer->SetReadPointer (0);
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);

  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)1);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// - ReadBytesAndMovePtr ()
test (ByteRingBuffer_ReadBytesAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  bool result = ByteRingBuffer::Create (length, defaultValue, pRingBuffer);
  assertTrue (result);
  uint8_t cache[10];

  //---------- Act & Assert ----------
  assertFalse (pRingBuffer->ReadBytesAndMovePtr (0,  nullptr, false));
  assertFalse (pRingBuffer->ReadBytesAndMovePtr (1,  nullptr, false));
  assertFalse (pRingBuffer->ReadBytesAndMovePtr (10, nullptr, false));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (0,  cache, false));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (1,  cache, false));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (2,  cache, false));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (9,  cache, false));
  assertFalse (pRingBuffer->ReadBytesAndMovePtr (10, cache, false));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (0,  cache, true));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (1,  cache, true));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (2,  cache, true));
  assertTrue  (pRingBuffer->ReadBytesAndMovePtr (9,  cache, true));
  assertFalse (pRingBuffer->ReadBytesAndMovePtr (10, cache, true));

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// - ReadBytesAndMovePtr ()
test (ByteRingBuffer_ReadBytesAndMovePtr_Test2)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  bool result = ByteRingBuffer::Create (length, defaultValue, pRingBuffer);
  assertTrue (result);

  uint8_t cacheSize = 10;
  uint8_t cache[10];

  //---------- Act & Assert ----------
  memset (&cache, 0x00, sizeof (cache));
  assertTrue (pRingBuffer->ReadBytesAndMovePtr (3, cache, false));
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)3);
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], index < 3 ? defaultValue : 0x00);

  memset (&cache, 0x00, sizeof (cache));
  assertTrue (pRingBuffer->ReadBytesAndMovePtr (7, cache, false));
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)1);
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], index < 7 ? defaultValue : 0x00);

  //---------- Arrange ----------
  pRingBuffer->SetWritePointer (2);  // -> writing starts at address 2
  for (uint16_t index = 1; index <= length; index++)
    pRingBuffer->WriteByteAndMovePtr (index * 11);  // -> 88 99 11 22 33 44 55 66 77

  //---------- Act & Assert ----------
  memset (&cache, 0x00, sizeof (cache));
  pRingBuffer->SetReadPointer (0);  // -> reading starts at address 0
  assertTrue (pRingBuffer->ReadBytesAndMovePtr (9, cache + 1, false));  // cache: 00 88 99 11 22 33 44 55 66 77
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)0);
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
  pRingBuffer->SetReadPointer (5);  // -> reading starts at address 5
  assertTrue (pRingBuffer->ReadBytesAndMovePtr (4, cache + 2, true));
  assertTrue (pRingBuffer->ReadBytesAndMovePtr (2, cache    , true));
  assertTrue (pRingBuffer->ReadBytesAndMovePtr (3, cache + 6, true));
  for (uint16_t index = 0; index < cacheSize; index++)
    assertEqual (cache[index], (uint8_t)((9 - index) * 11));
  assertEqual (pRingBuffer->get_CurrentReadAddress (), (uint16_t)5);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteByteAndMovePtr ()
// - MoveWritePointer ()
// - SetWritePointer ()
// - get_CurrentWriteAddress ()
test (ByteRingBuffer_WriteByteAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint8_t newValue1 = 0x12;
  uint8_t newValue2 = 0x34;
  uint8_t newValue3 = 0x56;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);
  for (uint16_t index = 0; index < length; index++)
  {
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
    assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);
  }
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);

  //---------- Act ----------
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)0);
  pRingBuffer->MoveWritePointer (3);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)3);
  pRingBuffer->WriteByteAndMovePtr (newValue1);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)4);
  pRingBuffer->MoveWritePointer ();
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)5);
  pRingBuffer->WriteByteAndMovePtr (newValue2);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)6);
  pRingBuffer->SetWritePointer (1);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)1);
  pRingBuffer->WriteByteAndMovePtr (newValue3);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)2);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length * 3; index++)
  {
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index % length == 3 ? newValue1 : index % length == 5 ? newValue2 : index % length == 1 ? newValue3 : defaultValue);
    assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)2);
  }

  //---------- Act ----------
  pRingBuffer->WriteByteAndMovePtr (newValue3);
  pRingBuffer->WriteByteAndMovePtr (newValue3);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)4);
  pRingBuffer->SetWritePointer (7);
  pRingBuffer->WriteByteAndMovePtr (newValue1);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)8);

  //---------- Assert ----------
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue3);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue3);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue3);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->SetWritePointer (3);
  assertEqual (pRingBuffer->get_CurrentWriteAddress (), (uint16_t)3);
  for (uint16_t index = 0; index < length; index++)
    pRingBuffer->WriteByteAndMovePtr (index * 10);

  //---------- Assert ----------
  pRingBuffer->SetReadPointer (0);
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), (uint8_t)(((index + 6) % length) * 10));

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteRange_FromStart ()
test (ByteRingBuffer_WriteRange_FromStart_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act & Assert ----------
  assertTrue  (pRingBuffer->WriteRange_FromStart ( 0,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_FromStart ( 1,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_FromStart ( 8,  0, 0x00));
  assertFalse (pRingBuffer->WriteRange_FromStart ( 9,  0, 0x00));
  assertFalse (pRingBuffer->WriteRange_FromStart (99,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_FromStart ( 0,  1, 0x00));
  assertTrue  (pRingBuffer->WriteRange_FromStart ( 0,  8, 0x00));
  assertTrue  (pRingBuffer->WriteRange_FromStart ( 0,  9, 0x00));
  assertFalse (pRingBuffer->WriteRange_FromStart ( 0, 10, 0x00));
  assertFalse (pRingBuffer->WriteRange_FromStart ( 0, 99, 0x00));

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteRange_FromStart ()
// - Clear ()
test (ByteRingBuffer_WriteRange_FromStart_Test2)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint8_t newValue1 = 0x12;
  uint8_t newValue2 = 0x34;
  uint8_t newValue3 = 0x56;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act ----------
  assertTrue (pRingBuffer->WriteRange_FromStart (0, 1, newValue1));
  assertTrue (pRingBuffer->WriteRange_FromStart (3, 4, newValue2));

  //---------- Assert ----------
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->Clear ();

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->WriteRange_FromStart (2, 4, newValue3);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index >= 2 && index < 6 ? newValue3 : defaultValue);

  //---------- Act ----------
  pRingBuffer->WriteRange_FromStart (0, 9, newValue1);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteRange_ToEnd ()
test (ByteRingBuffer_WriteRange_ToEnd_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act & Assert ----------
  assertTrue  (pRingBuffer->WriteRange_ToEnd ( 0,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_ToEnd ( 1,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_ToEnd ( 8,  0, 0x00));
  assertFalse (pRingBuffer->WriteRange_ToEnd ( 9,  0, 0x00));
  assertFalse (pRingBuffer->WriteRange_ToEnd (99,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_ToEnd ( 0,  1, 0x00));
  assertTrue  (pRingBuffer->WriteRange_ToEnd ( 0,  8, 0x00));
  assertTrue  (pRingBuffer->WriteRange_ToEnd ( 0,  9, 0x00));
  assertFalse (pRingBuffer->WriteRange_ToEnd ( 0, 10, 0x00));
  assertFalse (pRingBuffer->WriteRange_ToEnd ( 0, 99, 0x00));

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteRange_ToEnd ()
// - Clear ()
test (ByteRingBuffer_WriteRange_ToEnd_Test2)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint8_t newValue1 = 0x12;
  uint8_t newValue2 = 0x34;
  uint8_t newValue3 = 0x56;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act ----------
  assertTrue (pRingBuffer->WriteRange_ToEnd (0, 1, newValue1));
  assertTrue (pRingBuffer->WriteRange_ToEnd (6, 4, newValue2));

  //---------- Assert ----------
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);

  //---------- Act ----------
  pRingBuffer->Clear ();

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->WriteRange_ToEnd (2, 4, newValue3);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index < 2 || index > 6 ? newValue3 : defaultValue);

  //---------- Act ----------
  pRingBuffer->WriteRange_ToEnd (0, 9, newValue1);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteRange_StartToEnd ()
test (ByteRingBuffer_WriteRange_StartToEnd_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act & Assert ----------
  assertTrue  (pRingBuffer->WriteRange_StartToEnd ( 0,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_StartToEnd ( 1,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_StartToEnd ( 8,  0, 0x00));
  assertFalse (pRingBuffer->WriteRange_StartToEnd ( 9,  0, 0x00));
  assertFalse (pRingBuffer->WriteRange_StartToEnd (99,  0, 0x00));
  assertTrue  (pRingBuffer->WriteRange_StartToEnd ( 0,  1, 0x00));
  assertTrue  (pRingBuffer->WriteRange_StartToEnd ( 0,  8, 0x00));
  assertFalse (pRingBuffer->WriteRange_StartToEnd ( 0,  9, 0x00));
  assertFalse (pRingBuffer->WriteRange_StartToEnd ( 0, 99, 0x00));

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteRange_StartToEnd ()
// - Clear ()
test (ByteRingBuffer_WriteRange_StartToEnd_Test2)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint8_t newValue1 = 0x12;
  uint8_t newValue2 = 0x34;
  uint8_t newValue3 = 0x56;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act ----------
  assertTrue (pRingBuffer->WriteRange_StartToEnd (0, 1, newValue1));
  assertTrue (pRingBuffer->WriteRange_StartToEnd (3, 7, newValue2));

  //---------- Assert ----------
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue2);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);
  assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->Clear ();

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->WriteRange_StartToEnd (4, 2, newValue3);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index < 2 || index >= 4 ? newValue3 : defaultValue);

  //---------- Act ----------
  pRingBuffer->WriteRange_StartToEnd (2, 2, newValue1);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue1);

  //---------- Cleanup ----------
  delete (pRingBuffer);
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
