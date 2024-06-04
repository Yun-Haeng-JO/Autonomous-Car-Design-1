#define ledPin 13  // LED 디지털 핀 13번에 연결
#define LINE_DETECT_WHITE  1

#define ENA 6
#define IN1 11
#define IN2 10
#define IN3 8
#define IN4 9
#define ENB 7

int linesensor_data[5] = {0, 0, 0, 0, 0};  // 라인 센서 데이터를 저장할 배열
int linesensor_pin[5] = {14, 15, 16, 17, 18};   // 라인 센서가 연결된 핀 번호 배열
int motor_speed_offset = 0;  // 모터 속도 오프셋 (현재 사용되지 않음)

// 라인 센서 데이터를 읽는 함수
int read_digital_line_sensor(void)
{
  int i;
  int sum = 0;

  for(i = 0; i < 5; i++)
  {
    if(LINE_DETECT_WHITE == 0)
    {
      linesensor_data[i] = 1 - digitalRead(linesensor_pin[i]);
    }
    else
    {
      linesensor_data[i] = digitalRead(linesensor_pin[i]);
    }
    sum += linesensor_data[i];
  }

  if(sum == 5)
  {
    return sum;
  }
  else if(sum == 2)
  {
    if( (linesensor_data[3] == 1) && (linesensor_data[4] == 1) ) return 3;
    if( (linesensor_data[2] == 1) && (linesensor_data[3] == 1) ) return 1;
    if( (linesensor_data[1] == 1) && (linesensor_data[2] == 1) ) return -1;
    if( (linesensor_data[0] == 1) && (linesensor_data[1] == 1) ) return -3;
  }
  else if(sum == 1)
  {
    if((linesensor_data[0] == 1)) return -4;
    if((linesensor_data[1] == 1)) return -2;
    if((linesensor_data[2] == 1)) return 0;
    if((linesensor_data[3] == 1)) return 2;
    if((linesensor_data[4] == 1)) return 4;
  }
  else if(sum == 3)
  {
    return -10;
  }
  else
  {
    return -5;
  }
}

// 모터 제어 함수
void motor_R_control(int speed)
{
  if(speed >= 0)
  {
    analogWrite(ENA, speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else
  {
    analogWrite(ENA, -speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
}

void motor_L_control(int speed)
{
  if(speed >= 0)
  {
    analogWrite(ENB, speed);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else
  {
    analogWrite(ENB, -speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

// 초기 설정
void setup() {
  int i;

  pinMode(ledPin, OUTPUT);  // 13번 핀을 출력으로 설정

  for(i = 0; i < 5; i++)
  {
    pinMode(linesensor_pin[i], INPUT);  // 라인 센서 핀을 입력으로 설정
  }

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(2000000);
}

// 라인 따라가기 함수
void line_following(int line_type)
{
  switch(line_type)
  {
    case -4:
      motor_R_control(-200);
      motor_L_control(80);
      break;

    case -3:
      motor_R_control(-180);
      motor_L_control(40);
      break;

    case -2:
      motor_R_control(-160);
      motor_L_control(10);
      break;

    case -1:
      motor_R_control(-140);
      motor_L_control(20);
      break;

    case 0:  
      motor_R_control(-85);
      motor_L_control(85);
      break;

    case 1:
      motor_R_control(-20);
      motor_L_control(140);
      break;

    case 2:
      motor_R_control(-10);
      motor_L_control(160);
      break;

    case 3:
      motor_R_control(-40);
      motor_L_control(180);
      break;

    case 4:
      motor_R_control(-80);
      motor_L_control(200);
      break;

    case -10:
      motor_R_control(0);
      motor_L_control(0);
      break;
  }
}

// 메인 루프
void loop() {
  int i;
  int line_type = 0;

  line_type = read_digital_line_sensor();  // 라인 센서 데이터를 읽음

  Serial.print("Input data = ");
  for(i = 0; i < 5; i++)
  {
    Serial.print(linesensor_data[i]);
    Serial.print(" ");
  }
  Serial.println();

  line_following(line_type);  // 라인 따라가기 함수 호출
}
