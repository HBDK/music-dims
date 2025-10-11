using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.Models;

public record NavigationEntityDto([property: JsonPropertyName("id")]string Id, [property: JsonPropertyName("name")]string Name, [property: JsonPropertyName("link")]string Link);