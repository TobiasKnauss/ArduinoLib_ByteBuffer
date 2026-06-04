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
test (ByteRingBuffer_ReadByteAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  bool result = ByteRingBuffer::Create (length, defaultValue, pRingBuffer);
  assertTrue (result);

  //---------- Act & Assert ----------
  for (uint16_t index = 0; index < 2 * length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
// Test of the functions
// - WriteByteAndMovePtr ()
// - MoveWritePointer ()
// - SetWritePointer ()
test (ByteRingBuffer_WriteByteAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  pRingBuffer->MoveWritePointer (3);
  pRingBuffer->MoveWritePointer (2);
  pRingBuffer->SetWritePointer (1);
  pRingBuffer->WriteByteAndMovePtr (0x12);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length * 3; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index % length == 3 || index % length == 5 || index % length == 1 ? 0x12 : defaultValue);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
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
test (ByteRingBuffer_SetValue_FromStart_Test2)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xAB;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act ----------
  uint8_t newLength = 1;
  uint8_t newValue = 0x12;
  pRingBuffer->WriteRange_FromStart (0, newLength, newValue);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index < newLength ? newValue : defaultValue);

  //---------- Act ----------
  pRingBuffer->Clear ();

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), defaultValue);

  //---------- Act ----------
  newLength = 4;
  newValue = 0x23;
  pRingBuffer->WriteRange_FromStart (0, newLength, newValue);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), index < newLength ? newValue : defaultValue);

  //---------- Act ----------
  newLength = 9;
  newValue = 0x56;
  pRingBuffer->WriteRange_FromStart (0, newLength, newValue);

  //---------- Assert ----------
  for (uint16_t index = 0; index < length; index++)
    assertEqual (pRingBuffer->ReadByteAndMovePtr (), newValue);

  //---------- Cleanup ----------
  delete (pRingBuffer);
}

//--------------------------------------------------------------------
test (ByteRingBuffer_GetSetValueAndMovePtr_Test1)
{
  //---------- Arrange ----------
  uint8_t defaultValue = 0xFF;
  uint16_t length = 9;
  ByteRingBuffer* pRingBuffer = nullptr;
  assertTrue (ByteRingBuffer::Create (length, defaultValue, pRingBuffer));

  //---------- Act ----------
  // pRingBuffer->Write

  //---------- Assert ----------

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
