#include <string.h>
//#include "sdkconfig.h" // generated by "make menuconfig"



#define SDA2_PIN GPIO_NUM_18
#define SCL2_PIN GPIO_NUM_19

#define TAG_ECC608 "se050"

#define I2C_MASTER_ACK 0
#define I2C_MASTER_NACK 1
/*
void i2c_master_init()
{
	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = SDA2_PIN,
		.scl_io_num = SCL2_PIN,
		.sda_pullup_en = GPIO_PULLUP_DISABLE,
		.scl_pullup_en = GPIO_PULLUP_DISABLE,
		.master.clk_speed = 100000
		};
			
	i2c_param_config(I2C_NUM_0 , &i2c_config);
	i2c_driver_install(I2C_NUM_0 , I2C_MODE_MASTER, 0, 0, 0);


}

*/
#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <se05x_ecc_curves_values.h>
#include <se05x_tlv.h>
#include <string.h>
#include "ex_sss_auth.h"
#include "global_platf.h"
#include "smCom.h"
#include "fsl_sss_se05x_types.h"

//#include <nxLog_App.h>

static ex_sss_boot_ctx_t gex_sss_delete_all_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_delete_all_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

//static sss_status_t JCOP4_GetDataIdentify(void);
//static sss_status_t Applet_Identify(sss_se05x_session_t *pSession);


/*sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{

    sss_status_t status = kStatus_SSS_Fail;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pCtx->session;

    status = Applet_Identify(pSession);
    if (status != kStatus_SSS_Success)
        goto cleanup;

    status = kStatus_SSS_Success;
cleanup:
    return status;
}


#define CHECK_FEATURE_PRESENT(AppletConfig, ITEM)               \
    if (((kSE05x_AppletConfig_##ITEM) ==                        \
            ((AppletConfig) & (kSE05x_AppletConfig_##ITEM)))) { \
        LOG_I("With    " #ITEM);                                \
    }                                                           \
    else {                                                      \
        LOG_I("WithOut " #ITEM);                                \
    }

static sss_status_t Applet_Identify(sss_se05x_session_t *pSession)
{
    sss_status_t status = kStatus_SSS_Fail;
    smStatus_t sw_status;
    SE05x_Result_t result = kSE05x_Result_NA;

    sw_status = Se05x_API_CheckObjectExists(
        &pSession->s_ctx, kSE05x_AppletResID_UNIQUE_ID, &result);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_CheckObjectExists");
        goto cleanup;
    }
    uint8_t uid[SE050_MODULE_UNIQUE_ID_LEN];
    size_t uidLen = sizeof(uid);
    sw_status = Se05x_API_ReadObject(&pSession->s_ctx,
        kSE05x_AppletResID_UNIQUE_ID,
        0,
        (uint16_t)uidLen,
        uid,
        &uidLen);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_CheckObjectExists");
        goto cleanup;
    }
    LOG_W("#####################################################");
    LOG_AU8_I(uid, uidLen);

    // VersionInfo is a 7 - byte value consisting of :
    // - 1 - byte Major applet version
    // - 1 - byte Minor applet version
    // - 1 - byte patch applet version
    // - 2 - byte AppletConfig, indicating the supported applet features
    // - 2-byte Secure Box version: major version (MSB) concatenated with minor version (LSB).

    uint8_t applet_version[7];
    size_t applet_versionLen = sizeof(applet_version);
    sw_status = Se05x_API_GetVersion(
        &pSession->s_ctx, applet_version, &applet_versionLen);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_GetVersion");
        goto cleanup;
    }
    LOG_W("#####################################################");
    LOG_I("Applet Major = %d", applet_version[0]);
    LOG_I("Applet Minor = %d", applet_version[1]);
    LOG_I("Applet patch = %d", applet_version[2]);
    LOG_I("AppletConfig = %02X%02X", applet_version[3], applet_version[4]);
    {
        U16 AppletConfig = applet_version[3] << 8 | applet_version[4];
        CHECK_FEATURE_PRESENT(AppletConfig, ECDAA);
        CHECK_FEATURE_PRESENT(AppletConfig, ECDSA_ECDH_ECDHE);
        CHECK_FEATURE_PRESENT(AppletConfig, EDDSA);
        CHECK_FEATURE_PRESENT(AppletConfig, DH_MONT);
        CHECK_FEATURE_PRESENT(AppletConfig, HMAC);
        CHECK_FEATURE_PRESENT(AppletConfig, RSA_PLAIN);
        CHECK_FEATURE_PRESENT(AppletConfig, RSA_CRT);
        CHECK_FEATURE_PRESENT(AppletConfig, AES);
        CHECK_FEATURE_PRESENT(AppletConfig, DES);
        CHECK_FEATURE_PRESENT(AppletConfig, PBKDF);
        CHECK_FEATURE_PRESENT(AppletConfig, TLS);
        CHECK_FEATURE_PRESENT(AppletConfig, MIFARE);
        CHECK_FEATURE_PRESENT(AppletConfig, I2CM);
    }
    LOG_I("SecureBox = %02X%02X", applet_version[5], applet_version[6]);

    status = kStatus_SSS_Success;
cleanup:
    return status;
}


/*void app_main(void)
{
	i2c_master_init();
    main(0,NULL);
}*/