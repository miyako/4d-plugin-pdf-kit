#!/bin/sh
#DEVELOPER_ID_APPLICATION_CODE_SIGN_IDENTITY_NAME=`security find-identity -p codesigning -v | egrep 'Developer ID Application[^"]+' -o`
codesign --force --verbose --sign "$EXPANDED_CODE_SIGN_IDENTITY_NAME" "$CODESIGNING_FOLDER_PATH/Contents/manifest.json"
