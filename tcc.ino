// Do not remove the include below
#include"Anderson_Tcc.h"
#include<string.h>
#include<stdlib.h>

int index = 0;
intlock = 1;
char c;
booleansystemresponse;
charbuf[500];
charcall[] = "CallReady";
char ok[] = "OK";
charsmsreceived[] = "+CMTI: \"SM\"";
chargpspwr[] = "+CGPSPWR: 1";
chargpsrst[] = "+CGPSRST: 1";
charcmgr[] = "+CMGR";
char anderson[] = "82461423";
charfixed[] = "Fix";
charpos[] = "POS";
charalarmon[] = "Aon";
charalarmoff[] = "Aoff";
charbomboff[] = "Boff";
charbombon[] = "Bon";
intpinalarmon = 45;
intpinalarmoff = 47;
intpinbomb = 49;

charcd_google_maps[56];

voidfind_gps(char *nmea) {
	Stringcoordenada_gps = nmea;
	Stringlat_seg;
	Stringlong_seg;
	Stringlatitude_seg;
	Stringlongitude_seg;
	Stringcd_final_maps;

	charcd_lat_seg[8];
	charcd_long_seg[8];
	floatcoordenada_lat_seg;
	floatcoordenada_long_seg;
	lat_seg = coordenada_gps.substring(21, 28);
	long_seg = coordenada_gps.substring(35, 42);
	for (int i = 0; i < 7; i++) {
		cd_lat_seg[i] = lat_seg[i];
		cd_long_seg[i] = long_seg[i];
	}
	cd_lat_seg[7] = '\0';
	cd_long_seg[7] = '\0';
	coordenada_lat_seg = atof(cd_lat_seg) * 60;
	coordenada_long_seg = atof(cd_long_seg) * 60;

	char buffer[5];
	latitude_seg = dtostrf(coordenada_lat_seg, 2, 1, buffer);

	char buffer1[5];
	longitude_seg = dtostrf(coordenada_long_seg, 2, 1, buffer1);

	cd_final_maps = "http://maps.google.com/maps?q=-"
			+ coordenada_gps.substring(18, 19) + "+"
			+ coordenada_gps.substring(19, 21) + "+" + latitude_seg + ",-"
			+ coordenada_gps.substring(31, 33) + "+"
			+ coordenada_gps.substring(33, 35) + "+" + longitude_seg;
	for (int j = 0; j <cd_final_maps.length(); j++) {
		cd_google_maps[j] = cd_final_maps[j];
	}
	cd_google_maps[55] = '\0';
}

booleanfind_string(char *x, char *y) {
	int match = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	booleanresult = false;

	do {
		if (y[j] == x[i]) {
			k = i;
			do {
				if (y[j] != x[k]) {
					match = 1;
					j = 0;
				} else {
					j++;
					k++;
				}
			} while ((!match) && (j <strlen(y)));

			if (!match) {
				result = true;
			}

			match = 0;
		}
		i++;
	} while ((i <strlen(x)) &&(!result));

	match = 0;
	j = 0;
	i = 0;

	returnresult;
}

voidread_serial() {
	delay(200);
	while (Serial2.available() > 0) {
		c = Serial2.read();
		buf[index] = c;
		index++;
	}

	buf[index] = '\0';
	index = 0;
}

voidsend_sms(charnumber[8], char *text) {
	Serial2.print("AT+CMGS=");
	Serial2.print("\"+5592");
	Serial2.print(number);
	Serial2.println("\"");
	delay(1000);
	Serial2.print(text);
	delay(1000);
	Serial2.write(0x1A);
	delay(5000);
}

voidpower_on() {
	intstatus_pin = digitalRead(6);
	if (status_pin == LOW) {
		digitalWrite(5, HIGH);
		delay(1100);
		digitalWrite(5, LOW);
		delay(2400);
	}
}

voidsetup() {
	Serial.begin(9600);
	Serial2.begin(9600);
	pinMode(5, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(pinalarmon, OUTPUT);
	pinMode(pinalarmoff, OUTPUT);
	pinMode(pinbomb, OUTPUT);
	digitalWrite(5, HIGH);
	digitalWrite(39, LOW);
}

voiddel_sms() {
	Serial2.println("AT+CMGDA=\"DEL ALL\"");
	delay(1000);

	if (Serial2.available() > 0) {
		read_serial();
		systemresponse = find_string(buf, ok);
		if (systemresponse) {
			Serial.println("Todas as mensagens deletadas. Aguardando SMS...");
			send_sms(anderson, "Aguardando SMS de comando");
		}
	}
}
voidloop() {
	switch (lock) {
	//GSM boot
	case 1: {
		power_on();
		if (Serial2.available() > 0) {
			Serial.println("Recebendo dados do Modulo GSM/GPS...");
			read_serial();

			systemresponse = find_string(buf, call);

			if (systemresponse) {
				Serial.println("Inicialização concluída");
				Serial2.println("ATE0");
				delay(1000);
				Serial2.println("AT+CMGF=1");
				delay(1000);
				if (Serial2.available() > 0) {
					read_serial();
					systemresponse = find_string(buf, ok);
					if (systemresponse) {
						Serial.println("Modo texto OK.");
					}
				}

				Serial2.println("AT+CMGDA=\"DEL ALL\"");
				delay(1000);

				if (Serial2.available() > 0) {
					read_serial();
					systemresponse = find_string(buf, ok);

					if (systemresponse) {
						Serial.println("Todas as mensagens deletadas.");
						lock = 2;
					}
				}

				Serial2.println("AT+CGPSPWR=1");
				delay(1000);

				if (Serial2.available() > 0) {
					read_serial();
					systemresponse = find_string(buf, ok);

					if (systemresponse) {
						Serial.println("GPS ligado");
					}
				}

				Serial2.println("AT+CGPSRST=0");
				delay(1000);

				if (Serial2.available() > 0) {
					read_serial();
					systemresponse = find_string(buf, ok);

					if (systemresponse) {
						Serial.println("GPS em modo Cold");
						Serial.println("Aguardando SMS de comando.");
						send_sms(anderson, " Aguardando SMS de comando.");
						if (Serial2.available() > 0) {
							read_serial();
							systemresponse = find_string(buf, ok);

							if (systemresponse) {
								Serial.println("SMS enviado.");
								lock = 2;
							}
						}
					}
				}
			}
		}
	}
		break;

		//Menu
	case 2: {
		if (Serial2.available() > 0) {
			read_serial();

			systemresponse = find_string(buf, smsreceived);

			if (systemresponse) {
				Serial.println("SMS recebido.");
				Serial2.println("AT+CMGR=1");
				delay(1000);

				if (Serial2.available() > 0) {
					read_serial();

					if (find_string(buf, anderson)) {
						Serial.println("SMS de Anderson recebido.");
						Serial.println(buf);

						if (find_string(buf, pos)) {
							lock = 3;
						} elseif (find_string(buf, alarmon)) {
							lock = 4;
						} elseif (find_string(buf, alarmoff)) {
							lock = 5;
						} elseif (find_string(buf, bomboff)) {
							lock = 6;
						} elseif (find_string(buf, bombon)) {
							lock = 7;
						} else {

							Serial.println("Comando errado. Enviando SMS...");
							send_sms(anderson,
									"Comando errado. Enviando outro SMS.");
							if (Serial2.available() > 0) {
								read_serial();
								systemresponse = find_string(buf, ok);
								if (systemresponse) {
									Serial.println("SMS enviado.");
									del_sms();
								}
							}

						}
					} else {
						Serial.println("SMS recebido de usuário desconhecido");
						del_sms();
					}
				}
			}
		}

	}
		break; //end case 2

	case 3: {
		Serial.println("Comando confirmado. Recebendo coordenadas GPS...");
		send_sms(anderson,
				"SMS confirmado. Recebendo posição GPS. Por favor aguarde...");

		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS enviado.");

			}
		}

		do {
			Serial2.println("AT+CGPSRST?");
			delay(5000);

			if (Serial2.available() > 0) {
				read_serial();
				Serial.println(buf);
				systemresponse = find_string(buf, ok);
			}
		} while(!systemresponse);

		do {
			Serial2.println("AT+CGPSSTATUS?");
			delay(5000);

			if (Serial2.available() > 0) {
				read_serial();
				Serial.println(buf);
				systemresponse = find_string(buf, fixed);
			}
			delay(2000);
		} while(!systemresponse);

		Serial2.println("AT+CGPSINF=32");
		delay(1000);

		if (Serial2.available() > 0) {
			read_serial();
			Serial.println(buf);
			find_gps(buf);
			Serial.println(cd_google_maps);
			send_sms(anderson, cd_google_maps);
			delay(200);
			del_sms();
		}
		lock = 2;
	}
		break;
	case 4: {
		Serial.println("Comando confirmado. Ativando alarme...");
		send_sms(anderson, "SMS confirmado. Ativando alarme. Por favor aguarde...");
		//lock = 2;
		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS enviado.");

			}
		}

		digitalWrite(pinalarmon, HIGH);
		delay(2000);
		digitalWrite(pinalarmon, LOW);
		delay(2000);
		send_sms(anderson, "Alarme ativado");

		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS enviado");
				del_sms();
			}
		}
		lock = 2;
	}
		break;
	case 5: {
		Serial.println("Comando confirmado. Desativando alarme...");
		send_sms(anderson, "SMS confirmado. Desativando alarme. Por favor aguarde...");
		//lock = 2;
		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS enviado.");

			}
		}

		digitalWrite(pinalarmoff, HIGH);
		delay(2000);
		digitalWrite(pinalarmoff, LOW);
		delay(2000);
		send_sms(anderson, "Alarme desativado");

		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS sent.");
				del_sms();
			}
		}
		lock = 2;
	}
		break;
	case 6: {
		Serial.println("Comando confirmado. Desativando bomba de gasolina.");
		send_sms(anderson,"SMS confirmado. Desativando bomba de gasolina. Por favor aguarde...");
		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS enviado.");

			}
		}

		digitalWrite(pinbomb, HIGH);
		delay(2000);
		send_sms(anderson, "Bomba de gasoline desativada");

		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS sent.");
				del_sms();
			}
		}
		lock = 2;
	}
		break;
	case 7: {
		Serial.println("Comando confirmado. Ativando bomba de gasolina...");
		send_sms(anderson,
				"SMS confirmado.Ativando bomba de gasolina. Por favor aguarde...");
		//lock = 2;
		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS sent.");
			}
		}
		digitalWrite(pinbomb, LOW);
		delay(2000);
		send_sms(anderson, "Bomba de gasoline desativada");

		if (Serial2.available() > 0) {
			read_serial();
			systemresponse = find_string(buf, ok);

			if (systemresponse) {
				Serial.println("SMS enviado.");
				del_sms();
			}
		}
		lock = 2;
	}
		break;

	} //end switch
} //end loop
