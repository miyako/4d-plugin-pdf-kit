/* --------------------------------------------------------------------------------
 #
 #	4DPlugin-PDF-Kit.h
 #	source generated by 4D Plugin Wizard
 #	Project : PDF Kit
 #	author : miyako
 #	2020/02/10
 #  
 # --------------------------------------------------------------------------------*/

#ifndef PLUGIN_PDF_KIT_H
#define PLUGIN_PDF_KIT_H

#include "4DPluginAPI.h"

#include "C_BLOB.h"
#include "C_LONGINT.h"
#include "C_TEXT.h"
#include "ARRAY_TEXT.h"

#import <Quartz/Quartz.h>

#pragma mark -

void PDF_Count_pages(PA_PluginParameters params);
void PDF_GET_PAGE(PA_PluginParameters params);
void PDF_SET_PAGE_ROTATION(PA_PluginParameters params);
void PDF_REMOVE_PAGE(PA_PluginParameters params);
void PDF_EXCHANGE_PAGES(PA_PluginParameters params);
void PDF_INSERT_PAGES(PA_PluginParameters params);
void PDF_GET_PAGE_ANNOTATION(PA_PluginParameters params);
void PDF_Data_from_picture(PA_PluginParameters params);
void PDF_GET_DOCUMENT_ATTRIBUTES(PA_PluginParameters params);
void PDF_SET_DOCUMENT_ATTRIBUTES(PA_PluginParameters params);
void PDF_Get_document_version(PA_PluginParameters params);
void PDF_Get_document_text(PA_PluginParameters params);

void PDF_EXCHANGE_PAGES2(PA_PluginParameters params);
void PDF_REMOVE_PAGE2(PA_PluginParameters params);
void PDF_INSERT_PAGES2(PA_PluginParameters params);

void PDF_SET_PAGE_ROTATION2(PA_PluginParameters params);
void PDF_GET_PAGE2(PA_PluginParameters params);

#endif /* PLUGIN_PDF_KIT_H */
