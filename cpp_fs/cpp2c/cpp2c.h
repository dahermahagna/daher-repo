#ifndef __CPP2C_H__
#define __CPP2C_H__

#include <stddef.h>


/*---------------------- typedef -------------------------------*/
/*-------- public transporter ---------*/

typedef struct PublicTransport PublicTransport_t;

typedef void (*PT_dtor_t)(PublicTransport_t* this_);
typedef void (*PT_dtordl_t)(PublicTransport_t *this_);
typedef void (*PT_display_t)(PublicTransport_t* this_);
typedef void (*PT_print_count_t)(void);
typedef int (*PT_get_id_t)(PublicTransport_t* this_);

/*------------- minibus ---------------*/

typedef struct MiniBus MiniBus_t;
typedef void (*MB_dtor_t)(MiniBus_t* this_);
typedef void (*MB_dtordl_t)(MiniBus_t *this_);
typedef void (*MB_display_t)(MiniBus_t* this_);
typedef void (*MB_wash_t)(MiniBus_t* this_,int minutes);

/*------------- taxi ---------------*/

typedef struct Taxi Taxi_t;
typedef void (*TA_dtor_t)(Taxi_t *this_);
typedef void (*TA_dtordl_t)(Taxi_t *this_);
typedef void (*TA_display_t)(Taxi_t *this_);

/*------------- specialstaxi ---------------*/

typedef struct SpecialTaxi SpecialTaxi_t;
typedef void (*STA_dtor_t)(SpecialTaxi_t *this_);
typedef void (*STA_dtordl_t)(SpecialTaxi_t *this_);
typedef void (*STA_display_t)(SpecialTaxi_t *this_);

/*----------------- function decleration -----------------------*/

/*-------- public transporter ---------*/

PublicTransport_t PTCtor(PublicTransport_t *this_);
PublicTransport_t *PTCtorAl();
void PTDtor(PublicTransport_t* this_);
void PTDtorDl(PublicTransport_t *this_);
PublicTransport_t PTCCtor(PublicTransport_t *this_, const PublicTransport_t *other_);
void PTPrintCount(void);
void PTDisplay(PublicTransport_t* this_);
int PTGetId(PublicTransport_t* this_);

/*------------- minibus ---------------*/

MiniBus_t MBCtor(MiniBus_t *this_);
MiniBus_t *MBCtorAl();
void MBDtor(MiniBus_t *this_);
void MBDtorDl(MiniBus_t *this_);
MiniBus_t MBCCtor(MiniBus_t *this_, const MiniBus_t *other_);
void MBDisplay(MiniBus_t* this_);
void Wash(MiniBus_t* this_, int minutes);

/*------------- taxi ---------------*/

Taxi_t TACtor(Taxi_t *this_);
Taxi_t *TACtorAl();
void TADtor(Taxi_t *this_);
void TADtorDl(Taxi_t *this_);
Taxi_t TACCtor(Taxi_t *this_, const Taxi_t *other_);
void TADisplay(Taxi_t *this_);

/*------------- specialtaxi ---------------*/

SpecialTaxi_t STACtor(SpecialTaxi_t *this_);
SpecialTaxi_t *STACtorAl();
void STADtor(SpecialTaxi_t *this_);
void STADtorDl(SpecialTaxi_t *this_);
SpecialTaxi_t STACCtor(SpecialTaxi_t *this_, const SpecialTaxi_t *other_);
void STADisplay(SpecialTaxi_t *this_);

#endif /*__CPP2C_H__*/