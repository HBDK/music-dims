using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.PostPlayAlbum.Models;

public record AlbumResponseDto([property: JsonPropertyName("id")]string Id, [property: JsonPropertyName("name")]string Name);