<template>
  <div class="card p-2">
    <div class="card-body">
      <h2>{{ $t("resource_card.resources") }}</h2>
      <br />
      <p>{{ $t("resource_card.resources_desc") }}</p>
      <div class="card-group p-4 align-items-center">
        <a
          class="btn btn-success m-1"
          href="https://docs.qq.com/aio/DSGdQc3htbFJjSFdO"
          target="_blank"
        >
          <i class="fas fa-fw fa-file-alt"></i> User Guide</a
        >
        <a
          class="btn btn-primary m-1"
          href="https://qm.qq.com/q/3tWBFVNZ"
          target="_blank"
        >
          <i class="fab fa-fw fa-qq"></i> Join My QQ Group</a
        >
        <a
          class="btn btn-secondary m-1"
          href="https://github.com/qiin2333/Sunshine-Foundation"
          target="_blank"
        >
          <i class="fab fa-fw fa-github"></i> Github</a
        >
        <a
          class="btn btn-secondary m-1"
          href="https://github.com/qiin2333/moonlight-android/releases/tag/shortcut"
          target="_blank"
        >
          <i class="fab fa-fw fa-github"></i> Moonlight Power Enhanced Edition for Android</a
        >
        <a
          class="btn btn-secondary m-1"
          href="https://github.com/qiin2333/moonlight-qt"
          target="_blank"
        >
          <i class="fab fa-fw fa-github"></i>Moonlight-PC</a
        >
        <a
          class="btn btn-secondary m-1"
          href="https://github.com/WACrown/moonlight-android"
          target="_blank"
        >
          <i class="fab fa-fw fa-github"></i> Moonlight Crown Version for Android</a
        >
        <a
          class="btn btn-secondary m-1"
          href="https://github.com/TrueZhuangJia/moonlight-ios-NativeMultiTouchPassthrough"
          target="_blank"
        >
          <i class="fab fa-fw fa-github"></i> Moonlight Expert Edition for iOS</a
        >
      </div>
    </div>
  </div>
  <!-- Legal -->
  <div class="card p-2 mt-4">
    <div class="card-body">
      <h2>{{ $t("resource_card.legal") }}</h2>
      <br />
      <p>{{ $t("resource_card.legal_desc") }}</p>
      <div class="card-group p-4 align-items-center">
        <a
          class="btn btn-danger m-1"
          href="https://github.com/LizardByte/Sunshine/blob/master/LICENSE"
          target="_blank"
        >
          <i class="fas fa-fw fa-file-alt"></i>
          {{ $t("resource_card.license") }}</a
        >
        <a
          class="btn btn-danger m-1"
          href="https://github.com/LizardByte/Sunshine/blob/master/NOTICE"
          target="_blank"
        >
          <i class="fas fa-fw fa-exclamation"></i>
          {{ $t("resource_card.third_party_notice") }}</a
        >
      </div>
    </div>
  </div>
</template>
