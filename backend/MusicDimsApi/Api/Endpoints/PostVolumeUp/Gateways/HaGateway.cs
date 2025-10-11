using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;

namespace MusicDimsApi.Endpoints.PostVolumeUp.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string VolumeUpRoute = "/api/services/media_player/volume_up";

    public async Task VolumeUp(string id)
    {
        var body = new VolumeUpPostBody($"media_player.{id}");
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(VolumeUpRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        var test = await response.Content.ReadAsStringAsync();
        response.EnsureSuccessStatusCode();
    }
}

public record VolumeUpPostBody([property: JsonPropertyName("entity_id")]string EntityId);
public interface IHaGateway
{
    Task VolumeUp(string id);
}