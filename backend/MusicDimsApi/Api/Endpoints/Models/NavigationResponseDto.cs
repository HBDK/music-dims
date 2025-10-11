using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.Models;

public record NavigationResponseDto([property: JsonPropertyName("items")]NavigationEntityDto[] Items, [property: JsonPropertyName("back_link")]string BackLink);