using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetArtists.Models;

public record ArtistResponseDto([property: JsonPropertyName("id")]string Id, [property: JsonPropertyName("name")]string Name);