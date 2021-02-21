package com.lhmodel.server.codegen;

import java.util.Vector;
import javax.json.Json;
import javax.json.stream.JsonParser;

public class SwaggerFormatType {
  public String swaggerType;
  public String customFormat;
  public String customType;
  public String includeFile;

  public SwaggerFormatType(
    String _swaggerType,
    String _customFormat,
    String _customType,
    String _includeFile ) {
    swaggerType = _swaggerType;
    customFormat = _customFormat;
    customType = _customType;
    includeFile = _includeFile;
  }

  static public Vector< SwaggerFormatType > LoadSwaggerFormatTypes(
    JsonParser jsonArraySwaggerFormatTypes ) throws Exception {
    Vector< SwaggerFormatType > swaggerFormatTypes = new Vector< SwaggerFormatType >();

    JsonParser.Event jsonEvent = jsonArraySwaggerFormatTypes.next();
    if( jsonEvent == JsonParser.Event.START_ARRAY ) {
      jsonEvent = jsonArraySwaggerFormatTypes.next();
      while( jsonEvent != JsonParser.Event.END_ARRAY ) {
        if( jsonEvent == JsonParser.Event.START_OBJECT ) {
          String swaggerType = null;
          String customFormat = null;
          String customType = null;
          String includeFile = null;

          jsonEvent = jsonArraySwaggerFormatTypes.next();
          while( jsonEvent != JsonParser.Event.END_OBJECT ) {
            if( jsonEvent == JsonParser.Event.KEY_NAME ) {
              String key = jsonArraySwaggerFormatTypes.getString();
              // System.out.println( "KEY[" + key + "]" );
              jsonEvent = jsonArraySwaggerFormatTypes.next();
              if( jsonEvent == JsonParser.Event.VALUE_STRING ) {
                String val = jsonArraySwaggerFormatTypes.getString();
                // System.out.println( "VAL[" + val + "]" );

                if( key.equals( "swaggerType" ) )
                    swaggerType = val;
                else if( key.equals( "format" ) )
                    customFormat = val;
                else if( key.equals( "cppType" ) )
                    customType = val;
                else if( key.equals( "includeFile" ) )
                    includeFile = val;
              } else {
                throw new Exception( "error while parsing" );
              }
            } else {
              throw new Exception( "error while parsing" );
            }

            jsonEvent = jsonArraySwaggerFormatTypes.next();
          }

          if( swaggerType == null ||
              swaggerType.length() <= 0 ) {
            throw new Exception( "custom format types file missing type" );
          }

          swaggerFormatTypes.add(
            new SwaggerFormatType(
              swaggerType, customFormat, customType, includeFile ) );
        } else {
          throw new Exception( "error while parsing" );
        }

        jsonEvent = jsonArraySwaggerFormatTypes.next();
      }
    }

    return swaggerFormatTypes;
  }
}


