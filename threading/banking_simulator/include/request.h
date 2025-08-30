#pragma once

typedef enum {
  GET
} RequestType;

typedef struct {
  RequestType   type;
} Request;