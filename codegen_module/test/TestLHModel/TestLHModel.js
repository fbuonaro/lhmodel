{
    "openapi":  "3.0.0",
    "info":
    {
        "title":        "LHModelTest Server Side Models",
        "description":  "LHModelTest Server Side Models",
        "version":      "1.0.0"
    },
    "servers":
    {
    },
    "paths":
    {
    },
    "components":
    {
        "schemas":
        {
            "ExampleModelA":
            {
                "type": "object",
                "properties":
                {
                    "member1":
                    {
                        "type": "number",
                        "format": "double"
                    },
                    "member2":
                    {
                        "type": "array",
                        "items":
                        {
                            "type": "string"
                        }
                    },
                    "member3":
                    {
                        "type": "string",
                        "format": "uuid"
                    }
                }
            },
            "ExampleModelB":
            {
                "type": "object",
                "properties":
                {
                    "member1":
                    {
                        "$ref": "#/components/schemas/ExampleModelA"
                    },
                    "member2":
                    {
                        "type": "array",
                        "items":
                        {
                            "$ref": "#/components/schemas/ExampleModelA"
                        }
                    }
                }
            },
            "ExampleModelC":
            {
                "type": "object",
                "properties":
                {
                    "member1":
                    {
                        "type": "array",
                        "items":
                        {
                            "type": "array",
                            "items":
                            {
                                "$ref": "#/components/schemas/ExampleModelB"
                            }
                        }
                    }
                }
            },
            "ExampleModelD":
            {
                "type": "object",
                "properties":
                {
                    "member1":
                    {
                        "type": "integer",
                        "format": "int64"
                    },
                    "member2":
                    {
                        "type": "array",
                        "items":
                        {
                            "type": "array",
                            "items":
                            {
                                "type": "object",
                                "additionalProperties":
                                {
                                    "type": "object",
                                    "additionalProperties":
                                    {
                                        "$ref": "#/components/schemas/ExampleModelA"
                                    }
                                }
                            }
                        }
                    },
                    "member3":
                    {
                        "type": "object",
                        "additionalProperties":
                        {
                            "type": "object",
                            "additionalProperties":
                            {
                                "type": "string"
                            }
                        }
                    }
                }
            }
        }
    }
}
