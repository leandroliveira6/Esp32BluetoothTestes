#include "BluetoothSerial.h"

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void actions(String mensagem);

BluetoothSerial SerialBT;
int pinoLed = 25;

void setup() {
  Serial.begin(115200);

  pinMode(pinoLed, OUTPUT);
  
  SerialBT.register_callback(callback);
  if(!SerialBT.begin("Esp32BluetoothTestes")){
    Serial.println("ERRO: Houve um problema ao iniciar o bluetooth");
  }
  Serial.println("SUCESSO: Dispositivo iniciado e pronto para uso");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  delay(20);
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  switch(event){
    case ESP_SPP_INIT_EVT:
      Serial.println("1) Bluetooth iniciado");
      break;
    case ESP_SPP_START_EVT:
      Serial.println("2) Servidor iniciado");
      break;
    case ESP_SPP_SRV_OPEN_EVT:
      Serial.println("3) Cliente abriu uma conexão. "+String(SerialBT.hasClient()));
      break;
    case ESP_SPP_CLOSE_EVT:
      Serial.println("4) Cliente fechou uma conexão. "+String(SerialBT.hasClient()));
      break;
    case ESP_SPP_WRITE_EVT:
      Serial.println("6) Caracter da mensagem enviado.");
      break;
    case ESP_SPP_DATA_IND_EVT: {
      String mensagem = "";
      int numeroDeBytes = SerialBT.available();
      Serial.println("5) Mensagem recebida. Quantidade de caracteres: "+String(numeroDeBytes));
      for(int i=0; i<numeroDeBytes; i++){
        mensagem += char(SerialBT.read());
      }
      actions(mensagem);
      break;
    }
    
    case ESP_SPP_DISCOVERY_COMP_EVT:
      Serial.println("7) Discovery complete. "+String(SerialBT.hasClient()));
      break;
    case ESP_SPP_CONG_EVT:
      Serial.println("8) Connection congestion status changed. "+String(SerialBT.hasClient()));
      break;
    case ESP_SPP_CL_INIT_EVT:
      Serial.println("9) Client initiated a connection. "+String(SerialBT.hasClient()));
      break;
    case ESP_SPP_OPEN_EVT:
      Serial.println("10) Client connection open. "+String(SerialBT.hasClient()));
      break;
  }
}

void actions(String mensagem){
  Serial.println(mensagem);
  if(mensagem.substring(0,2) == "L0"){
    digitalWrite(pinoLed, LOW);
  }
  else if(mensagem.substring(0,2) == "L1"){
    digitalWrite(pinoLed, HIGH);
  }
}
