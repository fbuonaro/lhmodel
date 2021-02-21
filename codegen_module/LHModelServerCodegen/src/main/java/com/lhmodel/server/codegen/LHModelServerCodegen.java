package com.lhmodel.server.codegen;

import io.swagger.codegen.v3.CliOption;
import io.swagger.codegen.v3.CodegenModel;
import io.swagger.codegen.v3.CodegenProperty;
import io.swagger.codegen.v3.CodegenType;
import io.swagger.codegen.v3.generators.DefaultCodegenConfig;
import io.swagger.v3.oas.models.media.ArraySchema;
import io.swagger.v3.oas.models.media.MapSchema;
import io.swagger.v3.oas.models.media.Schema;
import io.swagger.v3.oas.models.media.StringSchema;

import java.io.File;
import java.io.FileInputStream;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.json.Json;
import javax.json.stream.JsonParser;

import java.lang.Thread;

// NOTE: comments regarding LHModelServerCodegen will be prefixed with 'NOTE' or 'TODO'

public class LHModelServerCodegen extends DefaultCodegenConfig {

  // source folder where to write the files
  protected String sourceFolder = "src";
  protected String lhmodelServerCodegenVersion = "0.0.1";

  // namespace of all models to be generated
  protected String namespace = "LHModelServerCodegenNS";

  // dir path of model
  protected String modelIncludeDir = null;

  // default/standard formats
  protected HashMap< String, HashMap< String, String > > formatsForType = null;

  // custom types
  protected String customFormatTypesFile = null;

  // whether the model filenames should be lower case
  protected boolean lcModelFilename = false;


  protected Logger LOGGER = LoggerFactory.getLogger( LHModelServerCodegen.class );

  protected String additionalPropertiesString = "";

  /**
   * Returns human-friendly help for the generator.  Provide the consumer with help
   * tips, parameters here
   * 
   * @return A string value for the help message
   */
  public String getHelp() {
    return "Generates C++ classes based on LHModels for a server-side library.";
  }

  public String getName() {
    return "LHModelServerCodegen";
  }

  public CodegenType getTag() {
    return CodegenType.SERVER;
  }


  public LHModelServerCodegen() {
    super();

    // set the output folder here
    outputFolder = "generated-code/LHModelServerCodegen";

    /**
     * Models.  You can write model files using the modelTemplateFiles map.
     * if you want to create one template for file, you can do so here.
     * for multiple files for model, just put another entry in the `modelTemplateFiles` with
     * a different extension
     */
    modelTemplateFiles.put( "model.h.mustache", ".h" );
    modelTemplateFiles.put( "model.cxx.mustache", ".cxx" );

    /**
     * Api classes.  You can write classes for each Api file with the apiTemplateFiles map.
     * as with models, add multiple entries with different extensions for multiple files per
     * class
    apiTemplateFiles.put(
      "api.mustache",   // the template to use
      ".sample");       // the extension for each file to write
     */

    /**
     * Template Location.  This is the location which templates will be read from.  The generator
     * will use the resource stream to attempt to read the templates.
     */
    templateDir = "LHModelServerCodegen";

    /**
     * Api Package.  Optional, if needed, this can be used in templates
    apiPackage = "io.swagger.server.api";
     */

    /**
     * Model Package.  Optional, if needed, this can be used in templates
     */
    modelPackage = "io.swagger.server.model";

    /**
     * Reserved words.  Override this with reserved words specific to your language
     */
    reservedWords = new HashSet<String> (
      Arrays.asList(
        "alignas",
        "alignof",
        "and",
        "and_eq",
        "asm",
        "atomic_cancel",
        "atomic_commit",
        "atomic_noexcept",
        "auto",
        "bitand",
        "bitor",
        "bool",
        "break",
        "case",
        "catch",
        "char",
        "char16_t",
        "char32_t",
        "class",
        "compl",
        "concept",
        "const",
        "constexpr",
        "const_cast",
        "continue",
        "decltype",
        "default",
        "delete",
        "do",
        "double",
        "dynamic_cast",
        "else",
        "enum",
        "explicit",
        "export",
        "extern",
        "false",
        "float",
        "for",
        "friend",
        "goto",
        "if",
        "inline",
        "int",
        "import",
        "long",
        "module",
        "mutable",
        "namespace",
        "new",
        "noexcept",
        "not",
        "not_eq",
        "nullptr",
        "operator",
        "or",
        "or_eq",
        "private",
        "protected",
        "public",
        "register",
        "reinterpret_cast",
        "requires",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "static_assert",
        "static_cast",
        "struct",
        "switch",
        "synchronized",
        "template",
        "this",
        "thread_local",
        "throw",
        "true",
        "try",
        "typedef",
        "typeid",
        "typename",
        "union",
        "unsigned",
        "using",
        "virtual",
        "void",
        "volatile",
        "wchar_t",
        "while",
        "xor",
        "xor_eq",
        "override ",
        "final ",
        "transaction_safe ",
        "transaction_safe_dynamic ",
        "if",
        "elif",
        "else",
        "endif",
        "defined",
        "ifdef",
        "ifndef",
        "define",
        "undef",
        "include",
        "line",
        "error",
        "pragma"
      )
    );

    /**
     * Supporting Files.  You can write single files for the generator with the
     * entire object tree available.  If the input file has a suffix of `.mustache
     * it will be processed by the template engine.  Otherwise, it will be copied
    supportingFiles.add(new SupportingFile("myFile.mustache",   // the input template or file
      "",                                                       // the destination folder, relative `outputFolder`
      "myFile.sample")                                          // the output file
    );
     */

    /**
     * Language Specific Primitives.  These types will not trigger imports by
     * the client generator
     */
    languageSpecificPrimitives = new HashSet<String>(
      Arrays.asList(
        "void",
        "std::nullptr_t",
        "bool",
        "char",
        "signed char",
        "unsigned char",
        "char32_t",
        "char16_t",
        "wchar_t",
        "float",
        "double",
        "long double",
        "short",
        "short int",
        "signed short",
        "signed short int",
        "unsigned short",
        "unsigned short int",
        "unsigned short int",
        "int",
        "signed",
        "signed int",
        "unsigned",
        "unsigned int",
        "long",
        "long int",
        "signed long",
        "signed long int",
        "unsigned long",
        "unsigned long int",
        "long long",
        "long long int",
        "signed long long",
        "signed long long int",
        "unsigned long long",
        "unsigned long long int"
      )
    );

    // NOTE - from swagger type/format to c++ type
    super.typeMapping = new HashMap<String, String>();
    typeMapping.put("array", "std::vector");
    typeMapping.put("boolean", "bool");
    typeMapping.put("integer", "signed long long int");
    typeMapping.put("list", "std::vector");
    typeMapping.put("object", "std::unordered_map");
    typeMapping.put("number", "long double");
    typeMapping.put("string", "std::string");

    formatsForType = new HashMap< String, HashMap< String, String > >();
    formatsForType.put( "array", new HashMap< String, String >() );

    formatsForType.put( "boolean", new HashMap< String, String >() );

    formatsForType.put( "integer", new HashMap< String, String >() );
    formatsForType.get( "integer" ).put("int32", "std::int32_t");
    formatsForType.get( "integer" ).put("int64", "std::int64_t");

    formatsForType.put( "list", new HashMap< String, String >() );

    formatsForType.put( "object", new HashMap< String, String >() );

    formatsForType.put( "number", new HashMap< String, String >() );
    formatsForType.get( "number" ).put("double", "double");
    formatsForType.get( "number" ).put("float", "float");

    formatsForType.put( "string", new HashMap< String, String >() );
    // formatsForType.get( "string" ).put("binary", ""); // TODO
    formatsForType.get( "string" ).put("byte", "unsigned char");
    formatsForType.get( "string" ).put("date", "std::tm");
    formatsForType.get( "string" ).put("date-time", "std::tm");
    // formatsForType.get( "string" ).put("file", ""); // TODO

    super.importMapping = new HashMap< String, String >();
    importMapping.put( "std::int32_t", "#include <cstdint>" );
    importMapping.put( "std::int64_t", "#include <cstdint>" );
    importMapping.put( "std::string", "#include <string>" );
    importMapping.put( "std::unordered_map", "#include <unordered_map>" );
    importMapping.put( "std::vector", "#include <vector>" );
    importMapping.put( "std::tm", "#include <ctime>" );

    // CLI options
    cliOptions.clear();
    addOption(
      "namespace",
      "namespace to generate models into",
      this.namespace);
    addOption(
      "modelIncludeDir",
      "directory path to prefix model include directives with",
      this.modelIncludeDir);
    addOption(
      "customFormatTypesFile",
      "path to json file containing custom format types; json should be formatted as { 'customFormatTypes': [ { 'swaggerType': <string swagger type>, 'format': <string format>, 'cppType': <string c++ type>, 'includeFile': (optional) <include file corresponding to type' } .. ] }",
      this.customFormatTypesFile);
    addOption(
      "lcModelFilename",
      "whether model filenames should be lower case");
  }

  @Override
  public void processOpts() {
    super.processOpts();

    additionalPropertiesString = additionalProperties.toString();
    additionalProperties.put( "withProperties", additionalPropertiesString );
    // additionalProperties.put("forVersion", openAPI.getOpenapi());
    additionalProperties.put("lhmodelServerCodegenVersion", lhmodelServerCodegenVersion);
  
    // NOTE - additional properties, template variables, options
    // {{namespace}}
    if( additionalProperties.containsKey( "namespace" ) ) {
      namespace = (String) additionalProperties.get( "namespace" );
    } else if( namespace.length() > 0 ) {
      additionalProperties.put( "namespace", namespace );
    } else {
       LOGGER.warn( "namespace is empty" );
      // throw new Exception( "namespace cannot be empty" );
    }

    try
    {
        // NOTE - override default types with new mappings/imports
        if( additionalProperties.containsKey( "customFormatTypesFile" ) ) {
          String customFormatTypesFile = (String) additionalProperties.get( "customFormatTypesFile" );
          JsonParser customSwaggerFormatTypesJson = Json.createParser( new FileInputStream(customFormatTypesFile) );
          Vector< SwaggerFormatType > customSwaggerFormatTypes = SwaggerFormatType.LoadSwaggerFormatTypes(
              customSwaggerFormatTypesJson );

          overrideTypeFormatMappings( customSwaggerFormatTypes );
        }
    } catch( java.io.FileNotFoundException e ) {
       LOGGER.warn( "customFormatTypesFile not found" );
    } catch( Exception e ) {
       LOGGER.warn( "error parsing customFormatTypesFile[" + e.toString() + "]" );
       e.printStackTrace();
    }

    // NOTE - additional properties, template variables, options
    // {{modelIncludeDir}}
    if( additionalProperties.containsKey( "modelIncludeDir" ) ) {
      modelIncludeDir = (String) additionalProperties.get( "modelIncludeDir" );
    }

    if( additionalProperties.containsKey( "lcModelFilename" ) ) {
      lcModelFilename = Boolean.valueOf(additionalProperties.get( "lcModelFilename" ).toString());
    }
    else {
      lcModelFilename = false;
    }

    if( modelIncludeDir != null ) {
      additionalProperties.put( "modelIncludeDir", modelIncludeDir );
    }
  }

    public void overrideTypeFormatMappings( Vector< SwaggerFormatType > customSwaggerFormatTypes ) {
        for( int i = 0; i < customSwaggerFormatTypes.size(); ++i ) {
            SwaggerFormatType swaggerFormatType = customSwaggerFormatTypes.get( i );

            if( typeMapping.containsKey( swaggerFormatType.swaggerType ) &&
                formatsForType.containsKey( swaggerFormatType.swaggerType ) ) {
                if( swaggerFormatType.customFormat != null &&
                    swaggerFormatType.customFormat.length() > 0 ) {
                    if( swaggerFormatType.customType == null ||
                        swaggerFormatType.customType.length() <= 0 ) {
                        // NOTE - custom type is null, erase the mapping for the format
                        formatsForType.get( swaggerFormatType.swaggerType ).remove(
                            swaggerFormatType.customFormat );
                    } else {
                        // NOTE - map the format to the specified type
                        formatsForType.get( swaggerFormatType.swaggerType ).put(
                            swaggerFormatType.customFormat,
                            swaggerFormatType.customType );
                    }
                } else {
                    // NOTE - no custom format, set the swagger type
                    typeMapping.put( swaggerFormatType.swaggerType, swaggerFormatType.customType );
                }

                // NOTE - set or remove include file import
                if( swaggerFormatType.includeFile != null &&
                    swaggerFormatType.includeFile.length() > 0 ) {
                    importMapping.put( swaggerFormatType.customType, swaggerFormatType.includeFile );
                } else {
                    importMapping.remove( swaggerFormatType.customType );
                }
            } else {
                LOGGER.warn( "unknown swagger type" );
            }
        }
    }

    protected void addOption(String key, String description, String defaultValue) {
        CliOption option = new CliOption( key, description );

        if( defaultValue != null ) {
            option.defaultValue( defaultValue );
        }
    
        cliOptions.add( option );
    }

    protected void addOption(String key, String description) {
        CliOption option = CliOption.newBoolean( key, description );

        cliOptions.add( option );
    }

    protected String getModelFilename(String name) {
        if( lcModelFilename )
            return name.toLowerCase();
        else
            return name;
    }


  @Override
  public String toModelImport(String name) {
      if (importMapping.containsKey(name)) {
          return importMapping.get(name);
      } else if (modelIncludeDir != null && (modelIncludeDir.length() > 0 )) {
          return "#include <" + modelIncludeDir + "/"  + getModelFilename(name) + ".h>";
      }
      else {
          return "#include <" + getModelFilename(name) + ".h>";
      }
  }

  @Override
  public String toModelFilename(String name) {
        return getModelFilename( name );
  }

  // ?????????
  // seems to work
  @Override
  protected void addImport(CodegenModel m, String type) {
      LOGGER.debug( "LHModelServerCodegen.addImport for " + m.toString() + " of " + type );
      // LOGGER.debug( Arrays.toString( Thread.currentThread().getStackTrace() ) );

      /* strange (bug?) behavior where all "containers" have an array import added to it
         not sure how to prevent it
         https://github.com/swagger-api/swagger-codegen-generators/blob/master/src/main/java/io/swagger/codegen/v3/generators/DefaultCodegenConfig.java#L3099-L3101
      */

      // if (type != null && needToImport(type) && importMapping.containsKey(type)) {
      if (type != null && needToImport(type)) {
          // m.imports.add(importMapping.get(type));
          m.imports.add(toModelImport(type));
          if(type.equals(getSchemaType(new MapSchema()))) // special case for mappings
              m.imports.add(toModelImport(getSchemaType(new StringSchema()))); // needs string import
      }
  }

  /**
   * Escapes a reserved word as defined in the `reservedWords` array. Handle escaping
   * those terms here.  This logic is only called if a variable matches the reserved words
   * 
   * @return the escaped term
   */
  @Override
  public String escapeReservedWord(String name) {
    return "_" + name;  // add an underscore to the name
  }

  /**
   * Location to write model files.  You can use the modelPackage() as defined when the class is
   * instantiated
   */
  public String modelFileFolder() {
    return outputFolder + "/" + sourceFolder + "/" + modelPackage().replace('.', File.separatorChar);
  }

  /**
   * Location to write api files.  You can use the apiPackage() as defined when the class is
   * instantiated
  @Override
  public String apiFileFolder() {
    return outputFolder + "/" + sourceFolder + "/" + apiPackage().replace('.', File.separatorChar);
  }
   */

  @Override
  public String getArgumentsLocation() {
    return null;
  }

  @Override
  protected String getTemplateDir() {
    return templateDir;
  }

  @Override
  public String getDefaultTemplateDir() {
    return templateDir;
  }

  /**
   * Optional - type declaration.  This is a String which is used by the templates to instantiate your
   * types.  There is typically special handling for different property types
   *
   * @return a string value used as the `dataType` field for model templates, `returnType` for api templates
   */
  @Override
  public String getTypeDeclaration(Schema p)
  {
    LOGGER.debug( "LHModelServerCodegen.getTypeDeclaration for " + p.toString() );
    // NOTE - logic for maps and vectors and lists and such to get the real type declaration

    // TODO - implement things like nullable, etc by recusively calling getTypeDeclaration
    //        after toggling off whatever property we are handling so we do not recurse infinitely
    //        can swagger handle the imports ???
    if( p instanceof ArraySchema ) {
      ArraySchema ap = ( ArraySchema ) p;
      Schema inner = ap.getItems();
      return getSchemaType( p ) + "<" + getTypeDeclaration( inner ) + ">";
    }
    // else if( p instanceof MapSchema && hasSchemaProperties(p)) {
    else if( p instanceof MapSchema ) {
      MapSchema mp = ( MapSchema ) p;
      Schema inner = ( Schema ) mp.getAdditionalProperties();
      return getSchemaType( p ) + "<" + getTypeDeclaration(new StringSchema()) + ", " + getTypeDeclaration( inner ) + ">";
    }

    return super.getTypeDeclaration( p );
  }

  /**
   * Optional - swagger type conversion.  This is used to map swagger types in a `Schema` into 
   * either language specific types via `typeMapping` or into complex models if there is not a mapping.
   *
   * @return a string value of the type or complex model for this property
   * @see io.swagger.v3.oas.models.media.Schema
   */
  @Override
  public String getSchemaType( Schema p ) {
    String type = null;
    String format = p.getFormat();
    String swaggerType = p.getType();

    if( ( swaggerType != null ) && typeMapping.containsKey( swaggerType.toLowerCase() ) ) {
      // assert formatsForType.containsKey ( swaggerType )
      if( ( format != null ) && formatsForType.get( swaggerType.toLowerCase() ).containsKey( format ) ) {
        type = formatsForType.get( swaggerType ).get( format );
      } else {
        type = typeMapping.get( swaggerType.toLowerCase() );
      }
    } else { // ???
      type = super.getSchemaType( p );
    }

    LOGGER.debug( "getSchemaType for " + p.toString() + " is " + type );

    return type;
  }

  @Override
  public Map<String, Object> postProcessAllModels(Map<String, Object> models) {
    Map<String, Object> processedModels =  super.postProcessAllModels(models);

    for (Map.Entry<String, Object> entry : processedModels.entrySet()) {
      String modelName = toModelName(entry.getKey());
      Map<String, Object> inner = (Map<String, Object>) entry.getValue();
      List<Map<String, Object>> innerModels = (List<Map<String, Object>>) inner.get("models");
      for (Map<String, Object> mo : innerModels) {
        CodegenModel codegenModel = (CodegenModel) mo.get("model");
        int varIndex = 0;

        codegenModel.getVendorExtensions().put( "x-includeDirective", toModelImport( codegenModel.getClassname() ) );
        codegenModel.getVendorExtensions().put( "x-numVars", codegenModel.getVars().size() );
        for (CodegenProperty var : codegenModel.getVars() ) {
          var.getVendorExtensions().put( "x-varIndex", varIndex++ );
        }
      }
    }

    return processedModels;
  }
}
