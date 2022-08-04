#include <gps.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, const char *argv[])
{
    int rc;
    struct timeval tv;
    char *message;

    struct gps_data_t gps_data;
    printf("running test...\n");
    if ((rc = gps_open("localhost", "2947", &gps_data)) == -1)
    {
        printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
        return EXIT_FAILURE;
    }

    gps_stream(&gps_data, WATCH_ENABLE, NULL);

    while (1)
    {
        /* read data */
        if ((rc = gps_read(&gps_data)) == -1)
        {
            printf("error occured reading gps data. code: %d, reason: %s\n", rc, gps_errstr(rc));
        }
        else
        {
            gettimeofday(&tv, NULL);
			printf("tv: %ld\n", tv.tv_sec);
            /* Check if get a fix data, Not verified */
            if (gps_data.fix.mode == MODE_2D) /* MODE_2D good for latitude/longitude */
            {

                printf("tv_secs: %f\n", gps_data.fix.time);
                printf("Time:%d,%d %d:%d",
                        gps_data.ais.type5.month, gps_data.ais.type5.day,
                        gps_data.ais.type5.hour, gps_data.ais.type5.minute);


                printf("Longitude: %f, Latitude: %f\n \
                        Altitude : %f,    Speed: %f\n",
                       gps_data.fix.longitude, gps_data.fix.latitude,
                       gps_data.fix.altitude, gps_data.fix.speed);
            }
            /**
                 *  Display satellites info from the GPS receiver. 
                 * It has been verified
                 */
            printf("Satellites visable: %d\n", gps_data.satellites_visible);
            for (int i = 0; i < gps_data.satellites_visible; i++)
            {
                printf("SV type:");
                switch (gps_data.skyview[i].PRN)
                {
                default:
                    printf("Not detect any start.\n");
                    break;
                case 0:
                    printf("GPS, ");
                    break;
                case 1:
                    printf("SABS, ");
                    break;
                case 2:
                    printf("Galileo, ");
                    break;
                case 3:
                    printf("Beidou, ");
                    break;
                case 4:
                    printf("Unknown, ");
                    break;
                case 5:
                    printf("QZSS, ");
                    break;
                case 6:
                    printf("Glonass, ");
                    break;
                }
                printf("SNR: %d, Elevation: %d\n",
                       (int)gps_data.skyview[i].ss,
                       gps_data.skyview[i].elevation);
            }
        }
        usleep(500000);
    }

    /* When you are done... */
    gps_stream(&gps_data, WATCH_DISABLE, NULL);

    return 0;
}

