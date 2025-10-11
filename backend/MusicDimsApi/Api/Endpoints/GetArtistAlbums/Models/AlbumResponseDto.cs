using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetArtistAlbums.Models;

public record AlbumResponseDto([property: JsonPropertyName("id")]string Id, [property: JsonPropertyName("name")]string Name);