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
            "ModelA":
            {
                "type": "object",
                "properties":
                {
                    "id":
                    {
                        "type": "integer",
                        "format": "int64"
                    },
                    "desc":
                    {
                        "type": "string"
                    },
                    "ts":
                    {
                        "type": "string",
                        "format": "date-time"
                    },
                    "weights":
                    {
                        "type": "array",
                        "items":
                        {
                            "type": "number"
                        }
                    }
                }
            },
            "PairA":
            {
                "type": "object",
                "properties":
                {
                    "id":
                    {
                        "type": "integer",
                        "format": "int64"
                    },
                    "modelA":
                    {
                        "$ref": "#/components/schemas/ModelA"
                    }
                }
            },
            "MappingA":
            {
                "type": "object",
                "properties":
                {
                    "innerMap":
                    {
                        "type": "object",
                        "additionalProperties":
                        {
                            "$ref": "#/components/schemas/ModelA"
                        }
                    }
                }
            },
            "MappingB":
            {
                "type": "object",
                "properties":
                {
                    "innerMap":
                    {
                        "type": "object",
                        "additionalProperties":
                        {
                            "$ref": "#/components/schemas/MappingA"
                        }
                    }
                }
            },
            "MappingC":
            {
                "type": "object",
                "properties":
                {
                    "innerMap":
                    {
                        "type": "object",
                        "additionalProperties":
                        {
                            "type": "object",
                            "additionalProperties":
                            {
                                "$ref": "#/components/schemas/ModelA"
                            }
                        }
                    }
                }
            },
            "ContainerA":
            {
                "type": "object",
                "properties":
                {
                    "arrA":
                    {
                        "type": "array",
                        "items":
                        {
                            "$ref": "#/components/schemas/ModelA"
                        }
                    }
                }
            }
        }
    }
}
