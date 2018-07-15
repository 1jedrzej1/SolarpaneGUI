#include "uart_thread.h"
#include <QtCore>


UART_thread::UART_thread(QObject *parent) :
    QThread(parent)
{
    init_connection = 0;
    init_error = false;
    mode = 0;
}

void UART_thread::run()
{
    UART_Init();
    InitConnection();
    CreateHistoryFile();
    int i = 0;

    while(1)
    {
        if(mode == 1)
        {
            GetCurrentIn12V();
            GetCurrentChargeAcu();
            GetCurrentDischargeAcu();
            //GetTensionChargeAcu();
            //GetTensionDischargeAcu();
            GetActuatorPosition();
            GetEnginePosition();
        }
        if(mode == 2)
        {
            GetCurrentIn12V();
            GetCurrentChargeAcu();
            GetCurrentDischargeAcu();
            //GetTensionChargeAcu();
            //GetTensionDischargeAcu();
            GetActuatorPosition();
            GetEnginePosition();
            GetFotoR_Right();
            GetFotoR_Left();
            GetFotoR_Down();
        }


        AddTempValue();
        if(i >= SAVE_TO_FILE_ITER)
        {
            temp_IIn12V = temp_IIn12V / SAVE_TO_FILE_ITER;
            temp_IChargeAcu = temp_IChargeAcu / SAVE_TO_FILE_ITER;
            temp_IDischargeAcu = temp_IDischargeAcu / SAVE_TO_FILE_ITER;
            temp_VChargeAcu = temp_VChargeAcu / SAVE_TO_FILE_ITER;
            temp_VDischargeAcu = temp_VDischargeAcu / SAVE_TO_FILE_ITER;
            UpdateHistoryFile();
            temp_IIn12V = 0;
            temp_IChargeAcu = 0;
            temp_IDischargeAcu = 0;
            temp_VChargeAcu = 0;
            temp_VDischargeAcu = 0;
            i=0;
        }


        i++;
        emit UpGUI();
        //UpdateGUI();
        this->msleep(500);
    }

}
void UART_thread::CreateHistoryFile()
{
    QTextStream PrintToFile(&file);
    bool bFileExist = false;

    temp_IIn12V = 0;
    temp_IChargeAcu = 0;
    temp_IDischargeAcu = 0;
    temp_VChargeAcu = 0;
    temp_VDischargeAcu = 0;
    QDate today = QDate::currentDate();
    QString RRRR;
    RRRR = RRRR.number(today.year());
    QString MM;
    if(today.month() <10) MM  = "0" + QString::number(today.month());
    QString DD;
    if(today.day() <10) DD  = "0" + QString::number(today.day());
    QString fileName = "/" + RRRR + "_" + MM + "_" + DD + "_History_File.txt";

    file.setFileName(FILE_PATH + fileName);
    if( file.exists() )
    {
        bFileExist = true;
    }
    file.open(QIODevice::Append | QIODevice::Text);

    if( !bFileExist ) PrintToFile << FILE_HEADER <<endl;
}

void UART_thread::UART_Init()
{
    //open UART
    uart0_filestream = open( "/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart0_filestream == -1)
    {
        qDebug() << "Unable to open UART";

    }
    else
    {
        qDebug() << "Opening /dev/ttyAMA0\n";
    }

    //configure UART
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CREAD;
    options.c_iflag = 0;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void UART_thread::UART_Transmit(char* data_transmit)
{
    qDebug() << "Sending:" << data_transmit;
    int wcount = 0;
    wcount = write(uart0_filestream, data_transmit, strlen(data_transmit));

    while( wcount < 6 )
    {
       wcount = write(uart0_filestream, data_transmit, strlen(data_transmit));
    }

    //data_transmit[wcount] = '\0';
}

void UART_thread::UART_Receive()
{
    //unsigned long int i = 0;
    int rcount = read(uart0_filestream, (void*)data_recieve, 6);
    while( rcount < 6/* && i < 65000*/)
    {
        rcount = read(uart0_filestream, (void*)data_recieve, 6);
        //i++;
    }
    data_recieve[rcount] = '\0';
    qDebug() << "Recieved: " << data_recieve;
}

void UART_thread::InitConnection()
{
    QMutexLocker locker(&mutex);
    if(mode == 1)
    {
        UART_Transmit((char*)"#I001*");
        UART_Receive();

        for (int i = 0; i < 4; i++)
        {
            if (strcmp(data_recieve, "#I001*") == 0 )
            {
                init_connection = 1;
                emit InitConnectionSend(init_connection);
                i = 4;
            }
            else if (strcmp(data_recieve, "#I001*") != 0 && i == 3)
            {
                init_error = true;
                emit InitConnectionSend(init_connection);
                //this->msleep(500);
            }
            else if (strcmp(data_recieve, "#I001*") != 0 && i != 3)
            {
                init_connection = 2;
                emit InitConnectionSend(init_connection);
                emit MessageSend(data_recieve);
                this->sleep(2);
                UART_Transmit((char*)"#I001*");
                UART_Receive();
                //this->sleep(2);
            }
        }

    }
    if(mode == 2)
    {
        UART_Transmit((char*)"#I002*");
        UART_Receive();

        for (int i = 0; i < 4; i++)
        {
            if (strcmp(data_recieve, "#I002*") == 0 )
            {
                init_connection = 1;
                emit InitConnectionSend(init_connection);
                i = 4;
            }
            else if (strcmp(data_recieve, "#I002*") != 0 && i == 3)
            {
                init_error = true;
                emit InitConnectionSend(init_connection);
                //this->msleep(500);
            }
            else if (strcmp(data_recieve, "#I002*") != 0 && i != 3)
            {
                init_connection = 2;
                emit InitConnectionSend(init_connection);
                emit MessageSend(data_recieve);
                this->sleep(2);
                UART_Transmit((char*)"#I002*");
                UART_Receive();
                //this->sleep(2);
            }
        }

    }
}

bool UART_thread::CheckFrame(char* data_recieve)
{
    if(data_recieve[0] =='#' && data_recieve[5] == '*' )
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
QString UART_thread::Wartosc_Pradu(char *data_recieve)
{
    char data[3];
    int wartoscRamki = 0;
    float wartoscPradu = 0;
    QString prad;

    //last_buffer = buffer;
    data[0] = data_recieve[2];
    data[1] = data_recieve[3];
    data[2] = data_recieve[4];
    wartoscRamki = atoi(data);
    wartoscPradu = (wartoscRamki * 264)/10;
    prad = QString::number(wartoscPradu);
    return prad;
}

QString UART_thread::Wartosc_Napiecia(char *data_recieve)
{
    char data[3];
    int wartoscRamki = 0;
    float wartoscNapiecia = 0;
    QString napiecie;

    //last_buffer = buffer;
    data[0] = data_recieve[2];
    data[1] = data_recieve[3];
    data[2] = data_recieve[4];
    wartoscRamki = atoi(data);
    wartoscNapiecia = (wartoscRamki * 9765625)/100000;
    napiecie = QString::number(wartoscNapiecia);
    return napiecie;
}
*/

char UART_thread::FramePar(char* tab)
{
    return tab[1];
}

int UART_thread::FrameValue(char* tab)
{
    char temp[3];
    temp[0] = tab[2];
    temp[1] = tab[3];
    temp[2] = tab[4];
    return atoi(temp);
}

void UART_thread::FrameInterpret(char *data)
{
    int WartoscRamki = FrameValue(data);
    char ParametrRamki = FramePar(data);
    float WartoscPrzeliczona;
    QString qWartoscPrzeliczona;
    //emit MessageSend(data_recieve);
    if(ParametrRamki == 'A')
    {
        if(WartoscRamki >= 0 && WartoscRamki <= 100)
        {
            ActuatorPosition = WartoscRamki;
        }
        else
        {
            emit MessageSend((char*)"Poza zakresem silownika");
        }
    }

    if(ParametrRamki == 'B')
    {
        //emit MessageSend((char*)"B");
        if(WartoscRamki >= 1 && WartoscRamki <= 30)
        {
            EnginePosition = WartoscRamki;
        }
        else
        {
            emit MessageSend((char*)"Poza zakresem silnika");
        }
    }

    if(ParametrRamki == 'C')
    {
        if(WartoscRamki >= 0 && WartoscRamki <= 512)
        {
            WartoscPrzeliczona = (WartoscRamki * 264)/10;
            qWartoscPrzeliczona = QString::number(WartoscPrzeliczona);
            IIn12V = qWartoscPrzeliczona;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki C");
        }
    }

    if(ParametrRamki == 'D')
    {
        if(WartoscRamki >= 0 && WartoscRamki <= 512)
        {
            WartoscPrzeliczona = (WartoscRamki * 264)/10;
            qWartoscPrzeliczona = QString::number(WartoscPrzeliczona);
            IChargeAcu = qWartoscPrzeliczona;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki D");
        }
    }


    if(ParametrRamki == 'E')
    {
        if(WartoscRamki >= 0 && WartoscRamki <= 512)
        {
            WartoscPrzeliczona = (WartoscRamki * 264)/10;
            qWartoscPrzeliczona = QString::number(WartoscPrzeliczona);
            IDischargeAcu = qWartoscPrzeliczona;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki E");
        }
    }

    if(ParametrRamki == 'F')
    {
        if(WartoscRamki >= 0 && WartoscRamki <= 512)
        {
            WartoscPrzeliczona = (WartoscRamki * 9765625)/100000;
            qWartoscPrzeliczona = QString::number(WartoscPrzeliczona);
            VChargeAcu = qWartoscPrzeliczona;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki F");
        }
    }

    if(ParametrRamki == 'G')
    {
        if(WartoscRamki >= 0 && WartoscRamki <= 512)
        {
            WartoscPrzeliczona = (WartoscRamki * 9765625)/100000;
            qWartoscPrzeliczona = QString::number(WartoscPrzeliczona);
            VDischargeAcu = qWartoscPrzeliczona;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki G");
        }
    }

    if(ParametrRamki == 'H')
    {
        if(WartoscRamki >= 0 && WartoscRamki  <= 100)
        {
            FotoR_Right = WartoscRamki;
        }
        else
        {
            emit MessageSend((char*)"BBledny zakres ramki H");
        }
    }

    if(ParametrRamki == 'J')
    {
        if(WartoscRamki >= 0 && WartoscRamki  <= 100)
        {
            FotoR_Left = WartoscRamki;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki J");
        }
    }

    if(ParametrRamki == 'K')
    {
        if(WartoscRamki >= 0 && WartoscRamki  <= 100)
        {
            FotoR_Down = WartoscRamki;
        }
        else
        {
            emit MessageSend((char*)"Bledny zakres ramki K");
        }
    }
    if(ParametrRamki == 'X')
    {
        if(WartoscRamki == 999)
        {
            emit MessageSend((char*)"Odebrano ramke w zlym formacie");
        }
        if(WartoscRamki == 998)
        {
            emit MessageSend((char*)"Odebrano ramke o niezdefiniowanym parametrze");
        }
        if(WartoscRamki == 997)
        {
            emit MessageSend((char*)"Blad podczas inicjalizacji silnika zrestartuj mikrokontroler");
        }
        if(WartoscRamki == 996)
        {
            emit MessageSend((char*)"Blad funkcj Detect_Smart Zrestartuj mikrokontroler");
        }
        if(WartoscRamki == 995)
        {
            emit MessageSend((char*)"Odebrano ramke o blednej wartosci ");
        }
    }

}

void UART_thread::GetCurrentIn12V()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_I12);
    UART_Receive();
    if (CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //IIn12V = Wartosc_Pradu(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"C Nie otrzymano pomiaru pradu silnikow");
    }
}

void UART_thread::GetCurrentChargeAcu()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ILAD);
    UART_Receive();
    if (CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //IChargeAcu = Wartosc_Pradu(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"D Nie otrzymano pomiaru pradu ladowania");
    }
}

void UART_thread::GetCurrentDischargeAcu()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_IROZL);
    UART_Receive();
    if (CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //IDischargeAcu = Wartosc_Pradu(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"E Nie otrzymano pomiaru pradu rozladowania");
    }
}

void UART_thread::GetTensionChargeAcu()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_VLAD);
    UART_Receive();

    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //VChargeAcu = Wartosc_Napiecia(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"F Nie otrzymano pomiaru napiecia ladowania");
    }
}

void UART_thread::GetTensionDischargeAcu()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_VROZL);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //VDischargeAcu = Wartosc_Napiecia(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"G Nie otrzymano pomiaru napiecia rozladowania");
    }
}
void UART_thread::SetManualMode()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_INIT_MANUAL);
    UART_Receive();
    if (CheckFrame(data_recieve) == true && data_recieve[1] == 'I')
    {
        emit MessageSend((char*)"Przelaczono na tryb MANUAL");
    }
    else
    {
        emit MessageSend((char*)"Niepoprawna ramka");
    }

}

void UART_thread::SetAutoMode()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_INIT_AUTO);
    UART_Receive();
    if (CheckFrame(data_recieve) == true && data_recieve[1] == 'I')
    {
        emit MessageSend((char*)"Przelaczono na tryb AUTO");
    }
    else
    {
        emit MessageSend((char*)"Niepoprawna ramka");
    }
}

void UART_thread::GetActuatorPosition()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ACTU_GETPOS);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //ActuatorPosition = ConvToActuatorPosition(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"A Nie otrzymano pomiaru pozycji silownikow");
    }
}
/*
int UART_thread::ConvToActuatorPosition(char *data_recieve)
{
    char data[3];
    int pozycja;

    data[0] = data_recieve[2];
    data[1] = data_recieve[3];
    data[2] = data_recieve[4];
    pozycja = atoi(data);
    return pozycja;
}
*/
void UART_thread::GetEnginePosition()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ENGI_GETPOS);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //EnginePosition = ConvToEnginePosition(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"B Nie otrzymano pozycji silnika");
    }
}
/*
int UART_thread::ConvToEnginePosition(char *data_recieve)
{
    char data[3];
    int pozycja;

    data[0] = data_recieve[2];
    data[1] = data_recieve[3];
    data[2] = data_recieve[4];
    pozycja = atoi(data);
    return pozycja;
}
*/

void UART_thread::EngineRight()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ENGI_MAN_RIGHT);
    UART_Receive();
    emit MessageSend(data_recieve);
}

void UART_thread::EngineLeft()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ENGI_MAN_LEFT);
    UART_Receive();
    emit MessageSend(data_recieve);
}

void UART_thread::ActuatorUp()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ACTU_MAN_UP);
    UART_Receive();
    emit MessageSend(data_recieve);
}

void UART_thread::ActuatorDown()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ACTU_MAN_DOWN);
    UART_Receive();
    emit MessageSend(data_recieve);
}

void UART_thread::ActuatorStop()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_ACTU_MAN_STOP);
    UART_Receive();
    emit MessageSend(data_recieve);
}

void UART_thread::GetFotoR_Right()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_FOTO_R);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
    }
    else
    {
        emit MessageSend((char*)"Nie otrzymano pomiaru fotorezytora R");
    }
}

void UART_thread::GetFotoR_Left()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_FOTO_L);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
    }
    else
    {
        emit MessageSend((char*)"Nie otrzymano pomiaru fotorezytora L");
    }
}

void UART_thread::GetFotoR_Down()
{
    QMutexLocker locker(&mutex);
    UART_Transmit((char*)UART_RPI_FOTO_D);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
    }
    else
    {
        emit MessageSend((char*)"Nie otrzymano pomiaru fotorezytora D");
    }
}

/*
void UART_thread::UpdateGUI()
{
    emit IIn12VSend(IIn12V);
    emit IChargeAcuSend(IChargeAcu);
    emit IDischargeAcuSend(IDischargeAcu);
    emit VChargeAcuSend(VChargeAcu);
    emit VDischargeAcuSend(VDischargeAcu);

}*/

void UART_thread::AddTempValue()
{
    temp_IIn12V += IIn12V.toLong();
    temp_IChargeAcu += IChargeAcu.toLong();
    temp_IDischargeAcu += IDischargeAcu.toLong();
    temp_VChargeAcu += VChargeAcu.toLong();
    temp_VDischargeAcu += VDischargeAcu.toLong();
}

void UART_thread::UpdateHistoryFile()
{
    QTextStream PrintToFile(&file);
    PrintToFile << temp_IIn12V << "\t\t" << temp_IChargeAcu << "\t\t" << temp_IDischargeAcu << "\t\t" << temp_VChargeAcu << "\t\t" << temp_VDischargeAcu << endl;
}

/*
void UART_thread::onSliderValueSend(int SliderValue)
{
    QString SV = QString::number(SliderValue);
    QString frame;
    if(SV.length() == 1) frame = "#A00" + SV + "*";
    if(SV.length() == 2) frame = "#A0" + SV + "*";
    if(SV.length() == 3) frame = "#A" + SV + "*";
    QByteArray ba = frame.toLocal8Bit();
    char* SliderV = ba.data();

    QMutexLocker locker(&mutex);
    UART_Transmit(SliderV);
    UART_Receive();
    if(CheckFrame(data_recieve) == true)
    {
        FrameInterpret(data_recieve);
        //ActuatorPosition = ConvToActuatorPosition(data_recieve);
    }
    else if (strlen(data_recieve) < 6)
    {
        //blad ramki
        emit MessageSend((char*)"A Nie otrzymano pomiaru pozycji silownikow");
    }
    //emit MessageSend(data_recieve);
}
*/
